#!/usr/bin/env python3
"""
teleop.launch.py — Teleoperation mode (IMU wearable controller)

Starts the full pipeline for controlling the arm with the ESP32 wearable:
  1. mqtt_imu_node   — bridges ESP32 sensor data (MQTT) → /odom + /imu/data
  2. MoveIt2         — robot state publisher + move_group
  3. esp32_controller — maps OTOS odometry to arm workspace, solves IK,
                        publishes servo commands to /arm/servo_commands
  4. pose_printer    — sole serial owner; relays /arm/servo_commands to Arduino

pose_printer is the ONLY node that opens the serial port. esp32_controller
publishes to /arm/servo_commands, which pose_printer forwards to hardware.

Usage:
  ros2 launch arm_bringup teleop.launch.py

Launch arguments:
  serial_port          Serial device for Arduino (default: /dev/ttyUSB0)
  dry_run              Log commands instead of sending to serial
                       (default: false)
  mqtt_broker          MQTT broker hostname (default: localhost)
  callback_skip_rate   Send command every N odometry callbacks (default: 5)
  lock_wrist           Lock wrist at default angle (default: false)
  lock_y_axis          Lock Y axis at default position (default: false)
  x_sensitivity        X-axis responsiveness multiplier (default: 3.0)
  default_y_position   Y position when Y locked, in metres (default: -1.2)
  default_z_position   Fixed Z height, in metres (default: 1.10)
  default_wrist_angle  Wrist angle when locked, in degrees (default: 90)

Example (hardware-free test):
  ros2 launch arm_bringup teleop.launch.py dry_run:=true

Recommended calibration from README:
  ros2 launch arm_bringup teleop.launch.py \\
    callback_skip_rate:=5 x_sensitivity:=3.0 \\
    lock_y_axis:=true default_y_position:=-1.2 \\
    lock_wrist:=true default_wrist_angle:=90 \\
    default_z_position:=1.10
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    # ── Launch arguments ──────────────────────────────────────────────────────
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
        "x_sensitivity", default_value="3.0",
        description="X-axis responsiveness (higher = more movement per sensor input)"
    )
    default_y_position_arg = DeclareLaunchArgument(
        "default_y_position", default_value="-1.2",
        description="Y position (metres) used when lock_y_axis=true"
    )
    default_z_position_arg = DeclareLaunchArgument(
        "default_z_position", default_value="1.10",
        description="Fixed Z height in metres"
    )
    default_wrist_angle_arg = DeclareLaunchArgument(
        "default_wrist_angle", default_value="90",
        description="Wrist servo angle (degrees) used when lock_wrist=true"
    )

    # ── MoveIt2 stack ─────────────────────────────────────────────────────────
    moveit_config_share = get_package_share_directory("robotic_arm_v3_config")
    moveit = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(moveit_config_share, "launch", "demo_with_controllers.launch.py")
        )
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
    esp32_controller = Node(
        package="esp32_controller",
        executable="esp32_controller",
        name="esp32_controller",
        parameters=[{
            "callback_skip_rate": LaunchConfiguration("callback_skip_rate"),
            "x_sensitivity": LaunchConfiguration("x_sensitivity"),
            "lock_y_axis": LaunchConfiguration("lock_y_axis"),
            "default_y_position": LaunchConfiguration("default_y_position"),
            "lock_wrist": LaunchConfiguration("lock_wrist"),
            "default_wrist_angle": LaunchConfiguration("default_wrist_angle"),
            "default_z_position": LaunchConfiguration("default_z_position"),
        }],
        output="screen",
    )

    # ── Serial arbitrator — sole owner of /dev/ttyUSB0 ────────────────────────
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
    )

    return LaunchDescription([
        serial_port_arg,
        dry_run_arg,
        mqtt_broker_arg,
        callback_skip_rate_arg,
        lock_wrist_arg,
        lock_y_axis_arg,
        x_sensitivity_arg,
        default_y_position_arg,
        default_z_position_arg,
        default_wrist_angle_arg,
        moveit,
        mqtt_imu,
        esp32_controller,
        pose_printer,
    ])
