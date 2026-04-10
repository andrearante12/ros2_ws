// Arduino Nano servo driver — smooth interpolation
//
// Receives "servoN=angle\n" commands over serial and smoothly moves servos
// toward target positions. No angle corrections — all offsets/inversions
// are handled by pose_printer on the ROS side.
//
// Hardware: PCA9685 PWM board at I2C address 0x40
// Baud rate: 115200

#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 125   // Minimum pulse length (0 degrees)
#define SERVOMAX 625   // Maximum pulse length (180 degrees)
#define NUM_SERVOS 6
#define MAX_STEP 2.0   // Max degrees per tick (at 50Hz → ~100°/sec)
#define LOOP_MS 20     // Loop period in ms (50Hz)

float current_angle[NUM_SERVOS];
float target_angle[NUM_SERVOS];
bool servo_active[NUM_SERVOS];  // only move servos that have received a command

void setup() {
  Serial.begin(115200);

  board1.begin();
  board1.setPWMFreq(60);

  for (int i = 0; i < NUM_SERVOS; i++) {
    current_angle[i] = 90.0;
    target_angle[i] = 90.0;
    servo_active[i] = false;
  }

  // Initialize hand rotation and grip
  board1.setPWM(4, 0, angleToPulse(0));
  board1.setPWM(5, 0, angleToPulse(45));
}

void loop() {
  // Read all available serial commands
  while (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int servo, angle;

    if (parseServoCommand(input, servo, angle)) {
      if (servo >= 0 && servo < NUM_SERVOS) {
        target_angle[servo] = (float)angle;
        if (!servo_active[servo]) {
          // First command for this servo — jump to position
          current_angle[servo] = (float)angle;
          board1.setPWM(servo, 0, angleToPulse(angle));
          servo_active[servo] = true;
        }
      }
    }
  }

  // Interpolate each active servo toward its target
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (!servo_active[i]) continue;

    // Servo 5 (gripper) snaps instantly
    if (i == 5) {
      if (current_angle[i] != target_angle[i]) {
        current_angle[i] = target_angle[i];
        board1.setPWM(i, 0, angleToPulse((int)round(current_angle[i])));
      }
      continue;
    }

    float diff = target_angle[i] - current_angle[i];
    if (abs(diff) < 0.5) {
      if (diff != 0.0) {
        current_angle[i] = target_angle[i];
        board1.setPWM(i, 0, angleToPulse((int)round(current_angle[i])));
      }
    } else {
      if (diff > MAX_STEP) diff = MAX_STEP;
      else if (diff < -MAX_STEP) diff = -MAX_STEP;
      current_angle[i] += diff;
      board1.setPWM(i, 0, angleToPulse((int)round(current_angle[i])));
    }
  }

  delay(LOOP_MS);
}

int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}

bool parseServoCommand(String cmd, int &servo, int &angle) {
  cmd.trim();

  if (!cmd.startsWith("servo")) return false;

  int eqIndex = cmd.indexOf('=');
  if (eqIndex == -1) return false;

  String servoStr = cmd.substring(5, eqIndex);
  String angleStr = cmd.substring(eqIndex + 1);

  for (char c : servoStr) {
    if (!isDigit(c)) return false;
  }

  int start = 0;
  if (angleStr.startsWith("-")) {
    if (angleStr.length() == 1) return false;
    start = 1;
  }

  for (int i = start; i < angleStr.length(); i++) {
    if (!isDigit(angleStr[i])) return false;
  }

  servo = servoStr.toInt();
  angle = angleStr.toInt();

  return true;
}
