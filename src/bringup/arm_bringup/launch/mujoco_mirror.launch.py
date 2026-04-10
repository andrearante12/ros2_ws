#!/usr/bin/env python3
"""
mujoco_mirror.launch.py — MuJoCo simulation with real arm mirroring

Runs the MuJoCo sim and bridges /joint_states to the Arduino over serial,
so the physical arm mirrors the simulated arm in real-time.

Supports swappable teleop input:
  teleop:=keyboard (default)  — WASD/QE keyboard control
  teleop:=esp32               — ESP32 wearable IMU controller

Usage:
  # Keyboard teleop, dry run (no serial):
  ros2 launch arm_bringup mujoco_mirror.launch.py dry_run:=true

  # Keyboard teleop, real arm:
  ros2 launch arm_bringup mujoco_mirror.launch.py

  # ESP32 wearable, real arm:
  ros2 launch arm_bringup mujoco_mirror.launch.py teleop:=esp32

  # Headless (no MuJoCo GUI):
  ros2 launch arm_bringup mujoco_mirror.launch.py headless:=true
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node


def generate_launch_description():
    bringup_share = get_package_share_directory("arm_bringup")

    # ── Launch arguments ──────────────────────────────────────────────────────
    teleop_arg = DeclareLaunchArgument(
        "teleop", default_value="keyboard",
        description="'keyboard' or 'esp32'"
    )
    headless_arg = DeclareLaunchArgument(
        "headless", default_value="false",
        description="Run MuJoCo without GUI"
    )
    serial_port_arg = DeclareLaunchArgument(
        "serial_port", default_value="/dev/ttyUSB0",
        description="Serial port connected to the Arduino"
    )
    dry_run_arg = DeclareLaunchArgument(
        "dry_run", default_value="false",
        description="Log servo commands instead of sending over serial"
    )
    mqtt_broker_arg = DeclareLaunchArgument(
        "mqtt_broker", default_value="localhost",
        description="MQTT broker hostname (esp32 mode only)"
    )

    teleop = LaunchConfiguration("teleop")
    is_keyboard = PythonExpression(["'", teleop, "' == 'keyboard'"])
    is_esp32 = PythonExpression(["'", teleop, "' == 'esp32'"])

    # ── MuJoCo simulation stack ───────────────────────────────────────────────
    mujoco = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(bringup_share, "launch", "mujoco.launch.py")
        ),
        launch_arguments={"headless": LaunchConfiguration("headless")}.items(),
    )

    # ── pose_printer: bridges /joint_states → Arduino serial ──────────────────
    # use_sim_time=False: timer fires at wall-clock 5Hz for steady serial rate
    pose_printer = Node(
        package="pose_printer",
        executable="pose_printer",
        name="pose_printer",
        parameters=[{
            "serial_port": LaunchConfiguration("serial_port"),
            "mode": "both",
            "dry_run": LaunchConfiguration("dry_run"),
        }],
        output="screen",
    )

    # ── Keyboard teleop (default) ─────────────────────────────────────────────
    keyboard_teleop = Node(
        package="moveit_controls",
        executable="keyboard_teleop",
        name="keyboard_teleop",
        parameters=[{"use_sim_time": True}],
        output="screen",
        prefix="xterm -e",
        condition=IfCondition(is_keyboard),
    )

    # ── ESP32 wearable teleop ─────────────────────────────────────────────────
    mqtt_imu = Node(
        package="mqtt_imu",
        executable="mqtt_imu_node",
        name="mqtt_imu_node",
        parameters=[{
            "mqtt_broker": LaunchConfiguration("mqtt_broker"),
        }],
        output="screen",
        condition=IfCondition(is_esp32),
    )

    esp32_controller = Node(
        package="esp32_controller",
        executable="esp32_controller",
        name="esp32_controller",
        parameters=[{
            "use_sim_time": True,
            "use_trajectory_controller": True,
        }],
        output="screen",
        condition=IfCondition(is_esp32),
    )

    return LaunchDescription([
        teleop_arg,
        headless_arg,
        serial_port_arg,
        dry_run_arg,
        mqtt_broker_arg,
        mujoco,
        pose_printer,
        keyboard_teleop,
        mqtt_imu,
        esp32_controller,
    ])
