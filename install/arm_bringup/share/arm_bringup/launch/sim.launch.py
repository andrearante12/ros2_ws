#!/usr/bin/env python3
"""
sim.launch.py — Simulation mode (no hardware required)

Starts MoveIt2 + RViz2 with the v3 arm model. Use this to:
  - Visualise the robot and test motion planning
  - Run the IK solver without any physical arm connected
  - Develop and debug without hardware

Usage:
  ros2 launch arm_bringup sim.launch.py
"""

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    moveit_config_share = get_package_share_directory("robotic_arm_v3_config")

    moveit_demo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(moveit_config_share, "launch", "demo_with_controllers.launch.py")
        )
    )

    return LaunchDescription([
        moveit_demo,
    ])
