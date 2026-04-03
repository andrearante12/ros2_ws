#!/usr/bin/env python3
"""
teleop.launch.py — Teleoperation mode (IMU wearable controller)

Starts the full pipeline for controlling the arm with the ESP32 wearable.
Supports three backends:

  backend:=real (default)
    1. mqtt_imu_node   — bridges ESP32 sensor data (MQTT) → /odom + /imu/data
    2. MoveIt2         — robot state publisher + move_group (mock hardware)
    3. esp32_controller — maps OTOS odometry to arm workspace, solves IK,
                          publishes servo commands to /arm/servo_commands
    4. pose_printer    — sole serial owner; relays /arm/servo_commands to Arduino

  backend:=gazebo
    1. mqtt_imu_node   — same as above
    2. Gazebo Harmonic — full physics simulation (includes MoveIt + ros2_control)
    3. esp32_controller — IK pipeline, publishes JointTrajectory to
                          /robotic_arm_controller/joint_trajectory (use_trajectory_controller=true)
    (no pose_printer)

  backend:=mujoco
    1. mqtt_imu_node   — same as above
    2. MuJoCo          — full physics simulation (includes MoveIt + ros2_control)
    3. esp32_controller — IK pipeline, publishes JointTrajectory to
                          /robotic_arm_controller/joint_trajectory (use_trajectory_controller=true)
    (no pose_printer)

pose_printer is the ONLY node that opens the serial port in real mode.
esp32_controller publishes to /arm/servo_commands in both modes.

Usage:
  # Real arm (default):
  ros2 launch arm_bringup teleop.launch.py

  # Gazebo simulation:
  ros2 launch arm_bringup teleop.launch.py backend:=gazebo

  # MuJoCo simulation:
  ros2 launch arm_bringup teleop.launch.py backend:=mujoco

  # Hardware-free test (real backend, no serial):
  ros2 launch arm_bringup teleop.launch.py dry_run:=true

Launch arguments:
  backend              'real', 'gazebo', or 'mujoco' (default: real)
  serial_port          Serial device for Arduino (default: /dev/ttyUSB0)
  dry_run              Log commands instead of sending to serial
                       (default: false, only applies to backend:=real)
  mqtt_broker          MQTT broker hostname (default: localhost)
  callback_skip_rate   Send command every N odometry callbacks (default: 5)
  lock_wrist           Lock wrist at default angle (default: false)
  lock_y_axis          Lock Y axis at default position (default: false)
  x_sensitivity        X-axis responsiveness multiplier (default: 6.0)
  y_sensitivity        Y-axis responsiveness multiplier (default: 6.0)
  default_y_position   Y position when Y locked, in metres (default: -0.03)
  default_z_position   Fixed Z height, in metres (default: 0.19)
  default_wrist_angle  Wrist angle when locked, in degrees (default: 90)
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node


def generate_launch_description():
    # ── Launch arguments ──────────────────────────────────────────────────────
    backend_arg = DeclareLaunchArgument(
        "backend", default_value="real",
        description="'real' to drive the physical arm, 'gazebo' or 'mujoco' for simulation"
    )
    serial_port_arg = DeclareLaunchArgument(
        "serial_port", default_value="/dev/ttyUSB0",
        description="Serial port connected to the Arduino Nano"
    )
    dry_run_arg = DeclareLaunchArgument(
        "dry_run", default_value="false",
        description="Log servo commands instead of sending over serial"
    )
    mqtt_broker_arg = DeclareLaunchArgument(
        "mqtt_broker", default_value="localhost",
        description="Hostname of the MQTT broker"
    )
    callback_skip_rate_arg = DeclareLaunchArgument(
        "callback_skip_rate", default_value="5",
        description="Send IK command every N odometry callbacks"
    )
    lock_wrist_arg = DeclareLaunchArgument(
        "lock_wrist", default_value="false",
        description="Lock wrist at default_wrist_angle"
    )
    lock_y_axis_arg = DeclareLaunchArgument(
        "lock_y_axis", default_value="false",
        description="Lock Y axis at default_y_position"
    )
    x_sensitivity_arg = DeclareLaunchArgument(
        "x_sensitivity", default_value="6.0",
        description="X-axis responsiveness (higher = more movement per sensor input)"
    )
    y_sensitivity_arg = DeclareLaunchArgument(
        "y_sensitivity", default_value="6.0",
        description="Y-axis responsiveness (higher = more movement per sensor input)"
    )
    default_y_position_arg = DeclareLaunchArgument(
        "default_y_position", default_value="-0.03",
        description="Y position (metres) used when lock_y_axis=true"
    )
    default_z_position_arg = DeclareLaunchArgument(
        "default_z_position", default_value="0.19",
        description="Fixed Z height in metres"
    )
    default_wrist_angle_arg = DeclareLaunchArgument(
        "default_wrist_angle", default_value="90",
        description="Wrist servo angle (degrees) used when lock_wrist=true"
    )

    backend = LaunchConfiguration("backend")
    is_real = PythonExpression(["'", backend, "' == 'real'"])
    is_gazebo = PythonExpression(["'", backend, "' == 'gazebo'"])
    is_mujoco = PythonExpression(["'", backend, "' == 'mujoco'"])
    # Both gazebo and mujoco publish /clock; use sim time for their controllers
    is_gazebo_sim = PythonExpression(["'", backend, "' in ('gazebo', 'mujoco')"])

    # ── MoveIt2 stack (real backend only — sim backends bring their own) ──────
    moveit_config_share = get_package_share_directory("robotic_arm_v3_config")
    moveit = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(moveit_config_share, "launch", "demo_with_controllers.launch.py")
        ),
        condition=IfCondition(is_real),
    )

    # ── Gazebo simulation stack (gazebo backend only) ─────────────────────────
    bringup_share = get_package_share_directory("arm_bringup")
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(bringup_share, "launch", "gazebo.launch.py")
        ),
        condition=IfCondition(is_gazebo),
    )

    # ── MuJoCo simulation stack (mujoco backend only) ─────────────────────────
    mujoco = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(bringup_share, "launch", "mujoco.launch.py")
        ),
        condition=IfCondition(is_mujoco),
    )

    # ── MQTT → ROS bridge ─────────────────────────────────────────────────────
    mqtt_imu = Node(
        package="mqtt_imu",
        executable="mqtt_imu_node",
        name="mqtt_imu_node",
        parameters=[{
            "mqtt_broker": LaunchConfiguration("mqtt_broker"),
        }],
        output="screen",
    )

    # ── Teleoperation controller ───────────────────────────────────────────────
    # use_sim_time=true when driving Gazebo so IK callbacks align with sim clock
    esp32_controller = Node(
        package="esp32_controller",
        executable="esp32_controller",
        name="esp32_controller",
        parameters=[{
            "callback_skip_rate": LaunchConfiguration("callback_skip_rate"),
            "x_sensitivity": LaunchConfiguration("x_sensitivity"),
            "y_sensitivity": LaunchConfiguration("y_sensitivity"),
            "lock_y_axis": LaunchConfiguration("lock_y_axis"),
            "default_y_position": LaunchConfiguration("default_y_position"),
            "lock_wrist": LaunchConfiguration("lock_wrist"),
            "default_wrist_angle": LaunchConfiguration("default_wrist_angle"),
            "default_z_position": LaunchConfiguration("default_z_position"),
            "use_sim_time": is_gazebo_sim,
            "use_trajectory_controller": is_gazebo_sim,
        }],
        output="screen",
    )

    # ── Serial arbitrator — sole owner of /dev/ttyUSB0 (real backend only) ────
    pose_printer = Node(
        package="pose_printer",
        executable="pose_printer",
        name="pose_printer",
        parameters=[{
            "serial_port": LaunchConfiguration("serial_port"),
            "mode": "direct",
            "dry_run": LaunchConfiguration("dry_run"),
        }],
        output="screen",
        condition=IfCondition(is_real),
    )

    return LaunchDescription([
        backend_arg,
        serial_port_arg,
        dry_run_arg,
        mqtt_broker_arg,
        callback_skip_rate_arg,
        lock_wrist_arg,
        lock_y_axis_arg,
        x_sensitivity_arg,
        y_sensitivity_arg,
        default_y_position_arg,
        default_z_position_arg,
        default_wrist_angle_arg,
        moveit,
        gazebo,
        mujoco,
        mqtt_imu,
        esp32_controller,
        pose_printer,
    ])
