#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from builtin_interfaces.msg import Time

import paho.mqtt.client as mqtt
import json

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "esp32/mpu6050"


class MqttImuNode(Node):

    def __init__(self):
        super().__init__("mqtt_imu_node")

        self.publisher_ = self.create_publisher(Imu, "/imu/raw", 10)

        self.get_logger().info("Starting MQTT IMU bridge")

        # MQTT client
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.client.connect(MQTT_BROKER, MQTT_PORT, 60)
        self.client.loop_start()

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.get_logger().info("Connected to MQTT broker")
            client.subscribe(MQTT_TOPIC)
            self.get_logger().info(f"Subscribed to {MQTT_TOPIC}")
        else:
            self.get_logger().error(f"MQTT connection failed (rc={rc})")

    def on_message(self, client, userdata, msg):
        try:
            data = json.loads(msg.payload.decode())

            imu_msg = Imu()

            # Timestamp
            imu_msg.header.stamp = self.get_clock().now().to_msg()
            imu_msg.header.frame_id = "imu_link"

            # Gyro → angular velocity (rad/s)
            imu_msg.angular_velocity.x = float(data["gyro"]["x"])
            imu_msg.angular_velocity.y = float(data["gyro"]["y"])
            imu_msg.angular_velocity.z = float(data["gyro"]["z"])

            # Accel → linear acceleration (m/s^2)
            imu_msg.linear_acceleration.x = float(data["acceleration"]["x"])
            imu_msg.linear_acceleration.y = float(data["acceleration"]["y"])
            imu_msg.linear_acceleration.z = float(data["acceleration"]["z"])

            # Orientation unknown (unless you compute quaternion)
            imu_msg.orientation_covariance[0] = -1.0

            self.publisher_.publish(imu_msg)

        except Exception as e:
            self.get_logger().error(f"Failed to parse MQTT IMU data: {e}")


def main():
    rclpy.init()
    node = MqttImuNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.client.loop_stop()
        node.client.disconnect()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
