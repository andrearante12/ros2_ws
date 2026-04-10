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
# Simulation only (MoveIt mock hardware) — no hardware required
ros2 launch arm_bringup sim.launch.py

# Gazebo physics simulation — full ros2_control + MoveIt + physics
ros2 launch arm_bringup gazebo.launch.py

# IK positioning — then in a 2nd terminal: ros2 run move_program move_program <x> <y> <z>
ros2 launch arm_bringup ik_positioning.launch.py
ros2 launch arm_bringup ik_positioning.launch.py dry_run:=true   # test without Arduino

# Teleoperation — ESP32 wearable controls arm in real time
ros2 launch arm_bringup teleop.launch.py
ros2 launch arm_bringup teleop.launch.py dry_run:=true           # test without Arduino
ros2 launch arm_bringup teleop.launch.py backend:=gazebo         # drive Gazebo instead

# MuJoCo sim-to-real mirroring — real arm follows MuJoCo in real time
ros2 launch arm_bringup mujoco_mirror.launch.py                    # keyboard teleop
ros2 launch arm_bringup mujoco_mirror.launch.py dry_run:=true      # test without Arduino
ros2 launch arm_bringup mujoco_mirror.launch.py teleop:=esp32      # ESP32 wearable

# Record demonstration for imitation learning (run alongside teleop or gazebo)
ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001
ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001 record_images:=true

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
- `pose_printer` — **Sole owner of `/dev/ttyUSB0`**. Subscribes to `/arm/servo_commands` (`std_msgs/String`) and/or `/joint_states` and writes to Arduino. In `joint_states` mode, uses `JOINT_SERVO_MAP` to convert all 6 joints (arm + gripper) from radians to servo degrees. Params: `mode` (`joint_states`|`direct`|`both`), `dry_run`, `serial_port`

**planning/**
- `move_program` — One-shot CLI: takes (x,y,z), runs gradient descent IK, publishes to `/arm/servo_commands`, exits. Also exports `include/move_program/gradient_ik.hpp`
- `esp32_controller` — Daemon: subscribes to `/odom`+`/imu/data`, solves IK continuously, publishes to `/arm/servo_commands`
- `moveit_controls` — Python nodes: `target_listener` (subscribes `/target_xyz`, executes via MoveIt KDL)

**perception/**
- `yolov5_detection` — ONNX chess piece detector; model at `models/chess_yolov11.onnx`
- `chess_data_collector` — Saves timestamped training images from camera

**interfaces/**
- `web_interface` — Flask server at `:5000`; streams camera feeds and joint states

## Gazebo Integration

`gazebo.launch.py` uses `gz_ros2_control/GazeboSimSystem` as the hardware interface instead of `mock_components/GenericSystem`. The URDF switches via the `use_sim` xacro argument:

- `robotic_arm_model_v3.urdf.xacro` — top-level xacro; accepts `use_sim:=false` (default)
- `robotic_arm_model_v3.ros2_control.xacro` — macro with `use_sim` param; selects hardware plugin conditionally
- `<gazebo>` plugin block (injected when `use_sim:=true`) loads `libgz_ros2_control-system.so` into Gazebo

`MoveItConfigsBuilder` in `gazebo.launch.py` passes `mappings={"use_sim": "true"}` to pick up the Gazebo interface. Controller spawning is sequenced via `RegisterEventHandler(OnProcessExit)`: spawn robot → `joint_state_broadcaster` → arm and hand controllers.

All nodes in Gazebo mode use `use_sim_time: True` and receive time from the `/clock` bridge (`ros_gz_bridge parameter_bridge`). Do NOT start a separate `ros2_control_node` — the Gazebo plugin acts as the controller manager.

Demos are recorded with `record_demo.launch.py` (separate terminal). Bags land in `~/demonstrations/<demo_name>/`.

## MuJoCo Sim-to-Real Mirroring

`mujoco_mirror.launch.py` runs the MuJoCo simulation and simultaneously drives the physical arm over serial, so the real arm mirrors the sim in real-time.

```
Keyboard/ESP32 → /move_to → move_server → MuJoCo sim → /joint_states
                                                              ↓
                                                    pose_printer (joint_states mode)
                                                              ↓
                                                    Arduino (/dev/ttyUSB0)
```

`pose_printer` runs with `use_sim_time: False` so its 5 Hz timer fires at wall-clock rate regardless of simulation speed. It reads the latest `/joint_states`, converts radians → degrees via `JOINT_SERVO_MAP` (all 6 servos including gripper), and writes `servoN=angle` to serial. The Arduino applies its own per-servo offset corrections (mounting offsets, inversions) to drive the physical servos.

In dry-run mode, servo values display as a single updating line in the terminal instead of being sent over serial.

```bash
# Keyboard teleop + real arm mirroring:
ros2 launch arm_bringup mujoco_mirror.launch.py

# Dry run (no serial — displays servo values in terminal):
ros2 launch arm_bringup mujoco_mirror.launch.py dry_run:=true

# ESP32 wearable instead of keyboard:
ros2 launch arm_bringup mujoco_mirror.launch.py teleop:=esp32

# Headless (no MuJoCo GUI):
ros2 launch arm_bringup mujoco_mirror.launch.py headless:=true
```

Launch arguments:
| Argument | Default | Description |
|---|---|---|
| `teleop` | `keyboard` | `keyboard` or `esp32` — input method for controlling the sim |
| `dry_run` | `false` | Log servo values instead of sending over serial |
| `serial_port` | `/dev/ttyUSB0` | Arduino serial port |
| `headless` | `false` | Run MuJoCo without GUI |
| `mqtt_broker` | `localhost` | MQTT broker (esp32 mode only) |

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

## Workspace Bounds (normalized, arm base at origin)

- X: −0.08 – 0.09 m
- Y: −0.18 – 0.10 m
- Z: fixed (configurable via `default_z_position`, default 0.19 m)
