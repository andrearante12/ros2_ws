# ImitationArm

A 6DOF robotic arm that learns skills by watching human demonstrations and mimicking movements. The system supports MuJoCo physics simulation, ESP32 wearable teleoperation, keyboard control, and MoveIt2 motion planning — all running on ROS2 Jazzy.

## System Requirements

- Ubuntu 24.04
- ROS2 Jazzy
- MoveIt2
- MuJoCo (via `mujoco_ros2_control`)

## Build

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash

# Full clean rebuild
rm -rf build install log
colcon build --cmake-args "-DCMAKE_EXE_LINKER_FLAGS=-lcurl"
source install/setup.bash
```

> The `-DCMAKE_EXE_LINKER_FLAGS=-lcurl` flag is required due to a linker quirk with `libresource_retriever`.

Quick rebuild for a single package:

```bash
colcon build --packages-select <package_name>
source install/setup.bash
```

Always source the workspace before running any launch command:

```bash
source /opt/ros/jazzy/setup.bash && source install/setup.bash
```

---

## MuJoCo Simulation

No hardware required. Runs MuJoCo physics, ros2_control, MoveIt2, and a persistent move server.

```bash
ros2 launch arm_bringup mujoco.launch.py
ros2 launch arm_bringup mujoco.launch.py headless:=true   # no GUI
```

---

## Teleoperation

### Keyboard (MuJoCo)

**Terminal 1:**
```bash
ros2 launch arm_bringup mujoco.launch.py
```

**Terminal 2:**
```bash
ros2 run moveit_controls keyboard_teleop
```

| Key | Action |
|---|---|
| W / S | Y forward / backward |
| A / D | X left / right |
| Q / E | Z up / down |
| O / C | Open / close gripper |
| R | Reset to center |

### ESP32 Glove (MuJoCo)

```bash
ros2 launch arm_bringup teleop.launch.py backend:=mujoco
```

### ESP32 Glove (Real Arm)

```bash
ros2 launch arm_bringup teleop.launch.py
ros2 launch arm_bringup teleop.launch.py dry_run:=true   # no Arduino needed
```

---

## Imitation Learning

Train a policy to autonomously lift a cube via behavioral cloning from human demonstrations. See the full training guide: **[docs/imitation_learning.md](docs/imitation_learning.md)**

Quick start:
```bash
# 1. Record demos (30-50 lifts)
ros2 launch arm_bringup mujoco_mirror.launch.py dry_run:=true   # Terminal 1
ros2 run moveit_controls demo_recorder                           # Terminal 2

# 2. Train (~1 min on CPU)
python3 -m moveit_controls.il.train

# 3. Deploy
ros2 launch arm_bringup policy_eval.launch.py                   # sim only
ros2 launch arm_bringup policy_eval.launch.py dry_run:=false     # with real arm
```

---

## Real Robot

### Sim-to-Real Mirroring

MuJoCo simulation drives the physical arm over serial in real time.

```bash
ros2 launch arm_bringup mujoco_mirror.launch.py                    # keyboard control
ros2 launch arm_bringup mujoco_mirror.launch.py teleop:=esp32      # ESP32 glove
ros2 launch arm_bringup mujoco_mirror.launch.py dry_run:=true      # test without Arduino
```

---

## Package Structure

```
src/
├── robot/
│   ├── robotic_arm_model_v3/    # URDF + mesh files
│   └── robotic_arm_v3_config/   # MoveIt2 config, MuJoCo scene, controllers
├── hardware/
│   ├── mqtt_imu/                # MQTT → /odom + /imu/data bridge
│   ├── pose_printer/            # Sole serial owner; relays /arm/servo_commands
│   └── esp32_viz/               # RViz marker visualization for ESP32 input
├── planning/
│   ├── move_program/            # Gradient descent IK + persistent move_server
│   ├── esp32_controller/        # Real-time teleoperation daemon
│   └── moveit_controls/         # MoveIt Python nodes + keyboard_teleop
├── perception/
│   ├── yolov5_detection/        # ONNX chess piece detector
│   └── chess_data_collector/    # Training image capture utility
├── interfaces/
│   └── web_interface/           # Flask monitoring dashboard
├── bringup/
│   └── arm_bringup/             # Integrated launch files (one per mode)
└── external/
    ├── mujoco_ros2_control/     # MuJoCo ↔ ros2_control plugin
    └── realsense-ros/           # RealSense camera driver
```

## Hardware

| Component | Connection | Details |
|---|---|---|
| Arduino Nano | `/dev/ttyUSB0` | 9600 baud; receives `servoN=angle` commands |
| ESP32 wearable | WiFi → MQTT | Publishes JSON to `esp32/dual_sensors` on `localhost:1883` |
| RealSense camera | USB | Required for vision mode |
