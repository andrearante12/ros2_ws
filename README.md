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

Runs the full arm in MuJoCo with real physics, ros2_control controllers, MoveIt2 planning, and a persistent move server. No physical hardware required.

```bash
ros2 launch arm_bringup mujoco.launch.py
```

This starts:
- **MuJoCo ros2_control node** — embeds physics simulation and acts as the controller manager (no separate simulator process)
- **MuJoCo Simulate GUI** — interactive 3D viewer (disable with `headless:=true`)
- **robot_state_publisher** — publishes URDF to TF
- **joint_state_broadcaster**, **robotic_arm_controller**, **hand_controller**, **finger_controller** — ros2_control controller chain
- **move_group** — MoveIt2 motion planning
- **gripper_control** — gripper open/close service
- **move_server** — persistent IK server accepting waypoints on `/move_to`

| Argument | Default | Description |
|---|---|---|
| `headless` | `false` | Run MuJoCo without GUI |

---

## ESP32 Teleoperation (MuJoCo)

Controls the simulated arm in real time using the ESP32 wearable (OTOS odometry + MPU-6050 IMU). The wearable position maps to the arm's XY workspace; IMU roll controls the wrist.

```bash
ros2 launch arm_bringup teleop.launch.py backend:=mujoco
```

This launches the full MuJoCo stack plus:
- **mqtt_imu_node** — bridges ESP32 MQTT sensor data to `/odom` + `/imu/data`
- **esp32_controller** — maps OTOS position to arm workspace, solves IK, publishes JointTrajectory commands

| Argument | Default | Description |
|---|---|---|
| `backend` | `real` | `real`, `gazebo`, or `mujoco` |
| `callback_skip_rate` | `5` | Send command every N odometry callbacks |
| `x_sensitivity` | `6.0` | X-axis responsiveness multiplier |
| `y_sensitivity` | `6.0` | Y-axis responsiveness multiplier |
| `lock_y_axis` | `false` | Lock Y at `default_y_position` |
| `lock_wrist` | `false` | Lock wrist at `default_wrist_angle` |
| `default_y_position` | `-0.03` | Y position (m) when Y locked |
| `default_z_position` | `0.19` | Fixed Z height (m) |
| `default_wrist_angle` | `90` | Wrist angle (deg) when locked |

---

## Keyboard Teleoperation (MuJoCo)

A drop-in replacement for the ESP32 wearable — control the arm from the keyboard. Publishes to the same `/move_to` topic as the ESP32 controller.

**Terminal 1** — start the MuJoCo simulation:

```bash
ros2 launch arm_bringup mujoco.launch.py
```

**Terminal 2** — start the keyboard teleop:

```bash
ros2 run moveit_controls keyboard_teleop
```

### Keyboard Controls

| Key | Action |
|---|---|
| W / S | Y forward / backward |
| A / D | X left / right |
| Q / E | Z up / down |
| O | Open gripper |
| C | Close gripper |
| R | Reset to center |
| Ctrl+C | Quit |

Step size is configurable:

```bash
ros2 run moveit_controls keyboard_teleop --ros-args -p step_size:=0.02
```

---

## ESP32 RViz Visualization

Visualize the ESP32 wearable controller's position and orientation in RViz2 without commanding the arm. Useful for debugging sensor input and calibrating the controller.

```bash
ros2 launch arm_bringup esp32_viz.launch.py
```

Then open RViz2, set **Fixed Frame** to `base_link`, and add a **MarkerArray** display subscribed to `/esp32_viz/markers`.

Published markers:
- **Green sphere** — target end-effector position in workspace
- **Orange arrow** — wrist orientation (roll + pitch from IMU)
- **Blue wireframe box** — workspace bounding volume

---

## Other Modes

### Simulation (MoveIt mock hardware)

```bash
ros2 launch arm_bringup sim.launch.py
```

### IK Positioning (real arm)

```bash
ros2 launch arm_bringup ik_positioning.launch.py
ros2 run move_program move_program <x> <y> <z>    # separate terminal
```

### Teleoperation (real arm)

```bash
ros2 launch arm_bringup teleop.launch.py
ros2 launch arm_bringup teleop.launch.py dry_run:=true   # no Arduino needed
```

### Gazebo Simulation

```bash
ros2 launch arm_bringup gazebo.launch.py
ros2 launch arm_bringup teleop.launch.py backend:=gazebo  # teleop in Gazebo
```

### Recording Demonstrations

```bash
ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001
ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001 record_images:=true
```

Bags are saved to `~/demonstrations/<demo_name>/`.

### Vision (chess piece detection)

```bash
ros2 launch arm_bringup vision.launch.py
ros2 launch arm_bringup vision.launch.py mode:=collect   # training data collection
```

Web dashboard at http://localhost:5000.

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
    ├── ros_gz/                  # Gazebo ↔ ROS2 bridge
    └── realsense-ros/           # RealSense camera driver
```

## Hardware

| Component | Connection | Details |
|---|---|---|
| Arduino Nano | `/dev/ttyUSB0` | 9600 baud; receives `servoN=angle` commands |
| ESP32 wearable | WiFi → MQTT | Publishes JSON to `esp32/dual_sensors` on `localhost:1883` |
| RealSense camera | USB | Required for vision mode |
