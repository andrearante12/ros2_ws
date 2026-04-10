#!/usr/bin/env python3
"""
policy_eval.launch.py — Run trained BC policy in MuJoCo simulation

Launches the MuJoCo sim stack and the policy executor node, which replaces
keyboard_teleop as the control source.

Usage:
  # Run policy in MuJoCo (with GUI):
  ros2 launch arm_bringup policy_eval.launch.py

  # Custom model path:
  ros2 launch arm_bringup policy_eval.launch.py model_path:=~/models/bc_cube_lift/best.pt

  # Headless (no MuJoCo GUI):
  ros2 launch arm_bringup policy_eval.launch.py headless:=true

  # With real arm mirroring:
  ros2 launch arm_bringup policy_eval.launch.py dry_run:=false
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    bringup_share = get_package_share_directory("arm_bringup")

    # ── Launch arguments ─────────────────────────────────────────────
    model_path_arg = DeclareLaunchArgument(
        "model_path",
        default_value=os.path.expanduser("~/models/bc_cube_lift/best.pt"),
        description="Path to trained BC policy checkpoint",
    )
    headless_arg = DeclareLaunchArgument(
        "headless", default_value="false",
        description="Run MuJoCo without GUI",
    )
    serial_port_arg = DeclareLaunchArgument(
        "serial_port", default_value="/dev/ttyUSB0",
        description="Serial port for real arm (only used when dry_run=false)",
    )
    dry_run_arg = DeclareLaunchArgument(
        "dry_run", default_value="true",
        description="Log servo commands instead of sending over serial",
    )

    # ── MuJoCo simulation stack ───────────���──────────────────────────
    mujoco = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(bringup_share, "launch", "mujoco.launch.py")
        ),
        launch_arguments={"headless": LaunchConfiguration("headless")}.items(),
    )

    # ── pose_printer (for real arm mirroring) ────────────────────────
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

    # ── Policy executor (replaces keyboard_teleop) ���──────────────────
    policy_executor = Node(
        package="moveit_controls",
        executable="policy_executor",
        name="policy_executor",
        parameters=[{
            "use_sim_time": True,
            "model_path": LaunchConfiguration("model_path"),
        }],
        output="screen",
    )

    return LaunchDescription([
        model_path_arg,
        headless_arg,
        serial_port_arg,
        dry_run_arg,
        mujoco,
        pose_printer,
        policy_executor,
    ])
