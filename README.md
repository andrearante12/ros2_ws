# ImitationArm

A 6DOF robotic arm that learns skills by watching human demonstrations and mimicking movements. The end goal is a generalised system where a human records a video of picking up and placing an object, uploads it to the arm, and the arm autonomously reproduces the exact motion.

## System Overview

A human trains the robot via a wearable device consisting of an ESP32 microcontroller and a MPU-6050 accelerometer/gyroscope. Data is streamed over WiFi to a central Raspberry Pi using MQTT. The Raspberry Pi handles calibration and signal processing.

Six MG996R PWM servomotors are driven by a PCA9685 16-channel PWM controller (I2C). Low-level actuator control is handled by an Arduino Nano. High-level control runs on a Raspberry Pi using ROS2 Jazzy.

A custom inverse kinematics solver (gradient descent) converts target coordinates (x, y, z) into servo angles. MoveIt2 validates paths via trajectory planning and collision detection. Commands are sent from the Raspberry Pi to the Arduino over serial.

## System Requirements

- Ubuntu 24.04
- ROS2 Jazzy
- MoveIt2 Harmonic

## Build

```bash
cd ~/ros2_ws

# Full clean rebuild
rm -rf build install log
source /opt/ros/jazzy/setup.bash
colcon build --cmake-args "-DCMAKE_EXE_LINKER_FLAGS=-lcurl"
source install/setup.bash
```

> **Note:** The `-DCMAKE_EXE_LINKER_FLAGS=-lcurl` flag is required due to a linking quirk in the system's `libresource_retriever`. This is a one-time setup step.

Quick rebuild for a single package:

```bash
colcon build --packages-select <package_name>
source install/setup.bash
```

---

## Operating Modes

Each mode is launched with a single command. Always source the workspace first:

```bash
source /opt/ros/jazzy/setup.bash && source install/setup.bash
```

---

### Simulation Mode (no hardware required)

Starts MoveIt2 + RViz2. Use this to visualise the robot, test motion planning, and develop without any physical hardware connected.

```bash
ros2 launch arm_bringup sim.launch.py
```

This starts:
- `robot_state_publisher` — publishes the URDF
- `move_group` — MoveIt2 planning server
- `ros2_control_node` — joint controller (simulated)
- `rviz2` — visualisation

---

### IK Positioning Mode

Moves the arm to a specific Cartesian coordinate using the gradient descent IK solver. `pose_printer` is the sole owner of the serial port and relays commands to the Arduino.

**Step 1** — Start the stack:

```bash
ros2 launch arm_bringup ik_positioning.launch.py
```

**Step 2** — In a separate terminal, send a target position:

```bash
source install/setup.bash
ros2 run move_program move_program <x> <y> <z>

# Example:
ros2 run move_program move_program 0.7 -1.2 1.145
```

**Workspace bounds:** X: 0.6–0.77 m · Y: −1.5 to −1.2 m

**Test without hardware:**

```bash
ros2 launch arm_bringup ik_positioning.launch.py dry_run:=true
```

When `dry_run:=true`, servo commands are logged to the terminal instead of being sent over serial — no Arduino needed.

**Launch arguments:**

| Argument | Default | Description |
|---|---|---|
| `serial_port` | `/dev/ttyUSB0` | Serial device for the Arduino Nano |
| `dry_run` | `false` | Log commands instead of sending over serial |

---

### Teleoperation Mode (IMU wearable controller)

Controls the arm in real time using the ESP32 wearable (OTOS odometry sensor + MPU-6050 IMU). The wearable position maps to the arm's XY workspace; IMU roll controls the wrist.

```bash
ros2 launch arm_bringup teleop.launch.py
```

This starts:
- `mqtt_imu_node` — bridges ESP32 sensor data from MQTT → `/odom` + `/imu/data`
- `move_group` — MoveIt2 (for robot model and state)
- `esp32_controller` — maps OTOS position to arm workspace, solves IK, publishes to `/arm/servo_commands`
- `pose_printer` (direct mode) — sole owner of `/dev/ttyUSB0`; relays `/arm/servo_commands` to Arduino

**Recommended calibration (from testing):**

```bash
ros2 launch arm_bringup teleop.launch.py \
  callback_skip_rate:=5 \
  x_sensitivity:=3.0 \
  lock_y_axis:=true \
  default_y_position:=-1.2 \
  lock_wrist:=true \
  default_wrist_angle:=90 \
  default_z_position:=1.10
```

**Test without hardware:**

```bash
ros2 launch arm_bringup teleop.launch.py dry_run:=true
```

**Launch arguments:**

| Argument | Default | Description |
|---|---|---|
| `serial_port` | `/dev/ttyUSB0` | Serial device for the Arduino Nano |
| `dry_run` | `false` | Log servo commands instead of sending over serial |
| `mqtt_broker` | `localhost` | Hostname of the MQTT broker |
| `callback_skip_rate` | `5` | Send IK command every N odometry callbacks (higher = slower) |
| `x_sensitivity` | `3.0` | X-axis responsiveness (higher = more movement per sensor input) |
| `lock_y_axis` | `false` | Lock Y at `default_y_position` |
| `lock_wrist` | `false` | Lock wrist at `default_wrist_angle` |
| `default_y_position` | `-1.2` | Y position (metres) used when `lock_y_axis:=true` |
| `default_z_position` | `1.10` | Fixed Z height in metres |
| `default_wrist_angle` | `90` | Wrist servo angle (degrees) used when `lock_wrist:=true` |

**Quick examples:**

```bash
# X-axis only (Y and wrist locked)
ros2 launch arm_bringup teleop.launch.py lock_y_axis:=true lock_wrist:=true

# High sensitivity, fast updates
ros2 launch arm_bringup teleop.launch.py callback_skip_rate:=2 x_sensitivity:=3.0

# Safe/slow for testing
ros2 launch arm_bringup teleop.launch.py callback_skip_rate:=10 x_sensitivity:=0.5
```

---

### Gazebo Simulation Mode (physics simulation)

Runs the full arm in Gazebo Harmonic with real physics, `ros2_control` controllers, MoveIt2, and RViz2. No physical hardware required. Use this for imitation learning demonstration collection and RL environment development.

```bash
ros2 launch arm_bringup gazebo.launch.py
```

This starts:
- `gz-sim` (Gazebo Harmonic) — physics world with table and target object
- `robot_state_publisher` — publishes URDF
- `ros_gz_bridge` — bridges Gazebo clock → ROS `/clock` topic
- `gz_ros2_control` plugin — controller manager inside Gazebo
- `joint_state_broadcaster`, `robotic_arm_controller`, `hand_controller` — spawned in sequence after robot loads
- `move_group` — MoveIt2 planning (sim time)
- `rviz2` — visualisation

**Launch arguments:**

| Argument | Default | Description |
|---|---|---|
| `world` | `arm_world.sdf` | Path to Gazebo world SDF file |
| `gz_headless` | `false` | Run Gazebo without GUI (server only) |

---

### Recording Demonstrations (imitation learning)

Records sensor and command data into a rosbag2 file for imitation learning. Run alongside any control launch file.

```bash
# Terminal 1 — start the arm (real or simulation)
ros2 launch arm_bringup teleop.launch.py              # real arm
ros2 launch arm_bringup teleop.launch.py backend:=gazebo  # Gazebo

# Terminal 2 — start recording
ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001
```

Always-recorded topics:
- `/joint_states` — observation: current joint positions (100 Hz)
- `/arm/servo_commands` — action: commanded servo positions
- `/clock`, `/tf`, `/tf_static` — time synchronisation and transforms
- `/yolo/detections` — object detections (if vision pipeline is running)

Optional (pass `record_images:=true`):
- `/camera/camera/color/image_raw`
- `/camera/camera/aligned_depth_to_color/image_raw`
- `/yolo/visualization`

Bags are saved to `~/demonstrations/<demo_name>/` and are readable with:
```bash
ros2 bag info ~/demonstrations/demo_001
ros2 bag play ~/demonstrations/demo_001
```

**Launch arguments:**

| Argument | Default | Description |
|---|---|---|
| `demo_name` | `demo` | Name for the bag file |
| `output_dir` | `~/demonstrations` | Parent directory for recordings |
| `record_images` | `false` | Also record camera RGB/depth topics (large files) |

---

### Teleoperation with Gazebo

Drive the Gazebo simulation using the wearable controller instead of the real arm:

```bash
ros2 launch arm_bringup teleop.launch.py backend:=gazebo
```

When `backend:=gazebo`, Gazebo is launched in place of the MoveIt-only stack, `pose_printer` is not started (no serial port opened), and `esp32_controller` uses sim time. All other wearable arguments (`lock_wrist`, `x_sensitivity`, etc.) work the same.

---

### Vision Mode (chess piece detection)

Runs the RealSense camera driver and YOLO object detection pipeline. Requires a RealSense RGB-D camera.

**Detection mode** (default) — runs the YOLO detector:

```bash
ros2 launch arm_bringup vision.launch.py
```

**Data collection mode** — saves training images to `~/chess_dataset/images/`:

```bash
ros2 launch arm_bringup vision.launch.py mode:=collect
```

Both modes start the web dashboard at **http://localhost:5000** — live camera feeds, joint states, and detection results.

Published topics when in detect mode:
- `/yolo/detections` — JSON string with class, bounding box, and depth
- `/yolo/visualization` — annotated camera frame

**Launch arguments:**

| Argument | Default | Description |
|---|---|---|
| `mode` | `detect` | `detect` or `collect` |
| `confidence_threshold` | `0.4` | YOLO detection confidence threshold (0.0–1.0) |
| `frame_skip` | `2` | Process every Nth camera frame |

---

## Serial Port Architecture

`pose_printer` is the **sole owner** of `/dev/ttyUSB0`. No other node opens the serial port directly.

- In **IK positioning mode**: `pose_printer` runs in `direct` mode — it subscribes to `/arm/servo_commands` and relays commands published by `move_program`.
- In **teleoperation mode**: `pose_printer` runs in `direct` mode — it relays commands published by `esp32_controller`.

This prevents the serial port conflict that would occur if both nodes tried to write simultaneously.

Topic: `/arm/servo_commands` (`std_msgs/String`) — newline-delimited servo commands, e.g.:
```
servo0=120
servo1=45
servo2=90
servo3=135
```

---

## Package Structure

```
src/
├── robot/
│   ├── robotic_arm_model_v3/    # URDF + mesh files
│   └── robotic_arm_v3_config/   # MoveIt2 config, launch files, controller YAML
├── hardware/
│   ├── mqtt_imu/                # MQTT → /odom + /imu/data bridge
│   └── pose_printer/            # Sole serial owner; relays /arm/servo_commands
├── planning/
│   ├── move_program/            # One-shot IK CLI (also exports gradient_ik.hpp)
│   ├── esp32_controller/        # Real-time teleoperation daemon
│   └── moveit_controls/         # MoveIt Python interface nodes
├── perception/
│   ├── yolov5_detection/        # ONNX chess piece detector
│   └── chess_data_collector/    # Training image capture utility
├── interfaces/
│   └── web_interface/           # Flask monitoring dashboard
├── bringup/
│   └── arm_bringup/             # Integrated launch files (one per mode)
└── external/
    ├── ros_gz/                  # Gazebo ↔ ROS2 bridge (git submodule)
    └── realsense-ros/           # RealSense camera driver (git submodule)
```

### Shared IK Header

`move_program` exports a shared header `move_program/gradient_ik.hpp` that both `move_program` and `esp32_controller` use. The `GradientDescentIK` class is defined once and shared via CMake's `target_include_directories`.

---

## Hardware Setup

| Component | Connection | Details |
|---|---|---|
| Arduino Nano | `/dev/ttyUSB0` | 9600 baud; receives `servoN=angle` commands |
| ESP32 wearable | WiFi → MQTT | Publishes JSON to `esp32/dual_sensors` on `localhost:1883` |
| RealSense camera | USB | Required for vision mode |

---

## Components

- PCA9685 Servo Driver: https://www.amazon.com/dp/B0CNVBWX2M
- Aluminum Body (w/out servos): https://www.amazon.com/dp/B01LW0LUPT
- MG996R Digital Servo Motor (×6): https://www.amazon.com/dp/B09LS7RB5J
- External Power Source (5V 6A): https://www.amazon.com/dp/B01D8FM4N4

## Future Direction

- Vision-assisted calibration for the wearable controller using D435 Depth Camera
- Reinforcement/imitation learning algorithm for autonomous task execution
