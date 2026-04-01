# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ROS2 workspace for a 6-DOF robotic arm (ImitationArm) with MoveIt2 motion planning, IMU-based teleoperation via ESP32 wearable, and YOLOv5 chess piece detection. The system uses ROS2 Jazzy on Ubuntu 24.04.

## Build Commands

```bash
# Source ROS2 environment first (required every session)
source /opt/ros/jazzy/setup.bash

# Full clean rebuild
rm -rf build install log
colcon build --cmake-args "-DCMAKE_EXE_LINKER_FLAGS=-lcurl"
source install/setup.bash

# Build a single package (after initial build)
colcon build --packages-select <package_name>
source install/setup.bash

# Python packages only (faster)
colcon build --symlink-install --packages-select pose_printer mqtt_imu
```

> The `-DCMAKE_EXE_LINKER_FLAGS=-lcurl` flag is required due to a linker quirk with `libresource_retriever` on this system.
> Skip the `ros_gz*` packages when building if Gazebo simulation isn't needed — they conflict with miniconda's Python 3.13.

## Launch Commands (One Per Mode)

```bash
# Simulation only — no hardware required
ros2 launch arm_bringup sim.launch.py

# IK positioning — then in a 2nd terminal: ros2 run move_program move_program <x> <y> <z>
ros2 launch arm_bringup ik_positioning.launch.py
ros2 launch arm_bringup ik_positioning.launch.py dry_run:=true   # test without Arduino

# Teleoperation — ESP32 wearable controls arm in real time
ros2 launch arm_bringup teleop.launch.py
ros2 launch arm_bringup teleop.launch.py dry_run:=true           # test without Arduino

# Vision — RealSense + YOLO detection + web dashboard at :5000
ros2 launch arm_bringup vision.launch.py
ros2 launch arm_bringup vision.launch.py mode:=collect           # training data collection
```

## Package Structure

Packages are grouped into subdirectories. `colcon` finds them recursively via `package.xml`.

```
src/
├── robot/        robotic_arm_model_v3, robotic_arm_v3_config
├── hardware/     mqtt_imu, pose_printer
├── planning/     move_program, esp32_controller, moveit_controls
├── perception/   yolov5_detection, chess_data_collector
├── interfaces/   web_interface
├── bringup/      arm_bringup  ← integrated launch files
└── external/     ros_gz, realsense-ros  ← git submodules
```

### Package Roles

**robot/**
- `robotic_arm_model_v3` — URDF + STL meshes (no nodes)
- `robotic_arm_v3_config` — MoveIt2 config (kinematics, controllers, RViz)

**hardware/**
- `mqtt_imu` — Bridges `esp32/dual_sensors` MQTT topic → `/odom` (OTOS) + `/imu/data` (MPU6050)
- `pose_printer` — **Sole owner of `/dev/ttyUSB0`**. Subscribes to `/arm/servo_commands` (`std_msgs/String`) and writes to Arduino. Params: `mode` (`joint_states`|`direct`|`both`), `dry_run`, `serial_port`

**planning/**
- `move_program` — One-shot CLI: takes (x,y,z), runs gradient descent IK, publishes to `/arm/servo_commands`, exits. Also exports `include/move_program/gradient_ik.hpp`
- `esp32_controller` — Daemon: subscribes to `/odom`+`/imu/data`, solves IK continuously, publishes to `/arm/servo_commands`
- `moveit_controls` — Python nodes: `target_listener` (subscribes `/target_xyz`, executes via MoveIt KDL)

**perception/**
- `yolov5_detection` — ONNX chess piece detector; model at `models/chess_yolov11.onnx`
- `chess_data_collector` — Saves timestamped training images from camera

**interfaces/**
- `web_interface` — Flask server at `:5000`; streams camera feeds and joint states

## Serial Port Architecture

`pose_printer` is the sole node that opens `/dev/ttyUSB0`. Other nodes publish servo commands as `std_msgs/String` to `/arm/servo_commands`. Format: newline-delimited `servoN=angle` strings.

- In IK positioning mode: `pose_printer mode:=direct`, `move_program` publishes then exits
- In teleop mode: `pose_printer mode:=direct`, `esp32_controller` publishes continuously

## Shared IK Header

`GradientDescentIK` lives in `src/planning/move_program/include/move_program/gradient_ik.hpp`. Both `move_program` and `esp32_controller` use it. When modifying IK behaviour, edit the header only — not either cpp file.

`esp32_controller/CMakeLists.txt` declares `find_package(move_program REQUIRED)` to get the include path.

## Key Topics

| Topic | Type | Publisher → Subscriber |
|---|---|---|
| `/arm/servo_commands` | `std_msgs/String` | `move_program` / `esp32_controller` → `pose_printer` |
| `/odom` | `nav_msgs/Odometry` | `mqtt_imu` → `esp32_controller` |
| `/imu/data` | `sensor_msgs/Imu` | `mqtt_imu` → `esp32_controller` |
| `/joint_states` | `sensor_msgs/JointState` | `ros2_control` → `pose_printer`, `esp32_controller`, `web_interface` |
| `/yolo/detections` | `std_msgs/String` | `yolov5_detection` → (future planner) |
| `/target_xyz` | `geometry_msgs/Point` | (external) → `target_listener` |

## Key Configuration Files

- `src/robot/robotic_arm_v3_config/config/kinematics.yaml` — KDL solver (resolution: 0.005 rad, timeout: 0.5s)
- `src/robot/robotic_arm_v3_config/config/ros2_controllers.yaml` — JointTrajectoryController at 100 Hz
- `src/robot/robotic_arm_v3_config/launch/demo_with_controllers.launch.py` — base launch file composed by `arm_bringup`

## Hardware

- Arduino Nano on `/dev/ttyUSB0` at 9600 baud — receives `servoN=angle\n` commands
- ESP32 wearable on WiFi → MQTT `localhost:1883`, topic `esp32/dual_sensors` (JSON)
- RealSense RGB-D camera — required for vision mode

## Workspace Bounds

- X: 0.60 – 0.77 m
- Y: −1.50 – −1.20 m
- Z: fixed (configurable via `default_z_position`, default 1.10 m)
