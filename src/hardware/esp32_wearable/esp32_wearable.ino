/*
 * Dual Sensor MQTT Publisher - OTOS + MPU6050
 *
 * Wearable hand controller for the ImitationArm robotic arm.
 * Reads OTOS (position) and MPU6050 (orientation) over I2C,
 * publishes combined JSON via MQTT at 10Hz.
 *
 * Data smoothing is handled on the ROS2 side (mqtt_imu_node.py)
 * via configurable EMA filters — no on-device calibration needed.
 *
 * I2C Bus (GPIO 21/22):
 *   OTOS  (PAA5160E1) 0x17 — X/Y position + heading (inches/degrees)
 *   MPU6050            0x68 — roll, pitch, accel, gyro
 *
 * MQTT topic: esp32/dual_sensors
 * JSON schema:
 *   { "otos":   {"x": float, "y": float, "h": float},
 *     "orient": {"r": float, "p": float, "y": float},
 *     "accel":  {"x": float, "y": float, "z": float},
 *     "gyro":   {"x": float, "y": float, "z": float},
 *     "ts":     uint32 }
 *
 * Dependencies (install via Arduino Library Manager):
 *   SparkFun Qwiic OTOS
 *   Adafruit MPU6050
 *   Adafruit Unified Sensor
 *   PubSubClient
 *   ArduinoJson
 */

#include "SparkFun_Qwiic_OTOS_Arduino_Library.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ── WiFi ─────────────────────────────────────────────────────────────────────
const char* ssid     = "Verizon_D4SHQL";
const char* password = "hurl-ranch9-raw";

// ── MQTT ──────────────────────────────────────────────────────────────────────
// Set mqtt_server to the IP of the machine running mosquitto (your Ubuntu desktop).
// Find it with: ip addr show | grep "inet " | grep -v 127
const char* mqtt_server = "192.168.1.199";
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "esp32/dual_sensors";

// ── Sensors ───────────────────────────────────────────────────────────────────
QwiicOTOS      myOtos;
Adafruit_MPU6050 mpu;

WiFiClient   espClient;
PubSubClient client(espClient);

// ─────────────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("======================================");
    Serial.println("ESP32 Wearable Controller");
    Serial.println("======================================");

    Wire.begin(21, 22);
    delay(100);

    // ── I2C scan ─────────────────────────────────────────────────────────────
    Serial.println("Scanning I2C bus...");
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.printf("  Found device at 0x%02X\n", addr);
        }
    }

    // ── OTOS ─────────────────────────────────────────────────────────────────
    Serial.println("Initializing OTOS...");
    if (!myOtos.begin()) {
        Serial.println("ERROR: OTOS not found on I2C bus!");
        while (1) delay(1000);
    }
    myOtos.resetTracking();  // Zero position reference at boot
    Serial.println("OTOS ready.");
    delay(200);  // Let I2C bus settle before initializing second device

    // ── MPU6050 ──────────────────────────────────────────────────────────────
    Serial.println("Initializing MPU6050...");
    if (!mpu.begin(0x68, &Wire)) {
        Serial.println("ERROR: MPU6050 not found on I2C bus!");
        while (1) delay(1000);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 ready.");

    // ── WiFi ─────────────────────────────────────────────────────────────────
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());

    // ── MQTT ─────────────────────────────────────────────────────────────────
    client.setServer(mqtt_server, mqtt_port);
    client.setBufferSize(512);

    Serial.println("======================================");
    Serial.println("Publishing to MQTT at 10Hz...");
    Serial.println("======================================");
}

void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    // ── Read sensors ─────────────────────────────────────────────────────────
    sfe_otos_pose2d_t otos_pos;
    myOtos.getPosition(otos_pos);

    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Roll and pitch from accelerometer (no gyro integration needed)
    float roll  = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
    float pitch = atan2(-accel.acceleration.x,
                        sqrt(accel.acceleration.y * accel.acceleration.y +
                             accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;

    // ── Build JSON ────────────────────────────────────────────────────────────
    StaticJsonDocument<512> doc;

    JsonObject otos = doc.createNestedObject("otos");
    otos["x"] = round(otos_pos.x * 100) / 100.0;  // 2 decimal places (inches)
    otos["y"] = round(otos_pos.y * 100) / 100.0;
    otos["h"] = round(otos_pos.h * 10)  / 10.0;   // 1 decimal place (degrees)

    JsonObject orient = doc.createNestedObject("orient");
    orient["r"] = round(roll  * 10) / 10.0;        // degrees
    orient["p"] = round(pitch * 10) / 10.0;
    orient["y"] = round(otos_pos.h * 10) / 10.0;  // yaw from OTOS heading

    JsonObject acc = doc.createNestedObject("accel");
    acc["x"] = round(accel.acceleration.x * 100) / 100.0;  // m/s²
    acc["y"] = round(accel.acceleration.y * 100) / 100.0;
    acc["z"] = round(accel.acceleration.z * 100) / 100.0;

    JsonObject gyr = doc.createNestedObject("gyro");
    gyr["x"] = round(gyro.gyro.x * 100) / 100.0;  // rad/s
    gyr["y"] = round(gyro.gyro.y * 100) / 100.0;
    gyr["z"] = round(gyro.gyro.z * 100) / 100.0;

    doc["ts"] = millis();

    // ── Publish ───────────────────────────────────────────────────────────────
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    if (client.publish(mqtt_topic, jsonBuffer)) {
        Serial.println(jsonBuffer);
    } else {
        Serial.println("ERROR: MQTT publish failed");
    }

    delay(100);  // 10Hz
}

void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT broker...");
        String clientId = "ESP32_Wearable-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println(" connected.");
        } else {
            Serial.print(" failed (rc=");
            Serial.print(client.state());
            Serial.println("), retrying in 5s");
            delay(5000);
        }
    }
}
