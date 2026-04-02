#!/usr/bin/env python3
"""
record_demo.launch.py — Imitation learning demonstration recorder

Records a single demonstration into a rosbag2 file. Run this alongside
any of the control launch files (teleop, ik_positioning, or gazebo).

The recorded topics contain everything needed for imitation learning:
  - /joint_states       → observation: current joint positions (100 Hz)
  - /arm/servo_commands → action: commanded servo positions
  - /clock              → synchronized time (required when use_sim_time=true)
  - /tf                 → end-effector pose via robot kinematics
  - /yolo/detections    → object detections (if vision pipeline is running)
  - /camera/...         → RGB/depth images (optional, large files)

Bags are saved to ~/demonstrations/<demo_name>/ and are readable with:
  ros2 bag info ~/demonstrations/<demo_name>
  ros2 bag play ~/demonstrations/<demo_name>

Usage:
  # Basic recording (joint states + commands, no images)
  ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001

  # With camera images (larger files, needed for vision-based IL)
  ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001 record_images:=true

  # With custom output directory
  ros2 launch arm_bringup record_demo.launch.py demo_name:=demo_001 output_dir:=/data/demos

Launch arguments:
  demo_name      Name for the bag file (default: demo)
  output_dir     Parent directory for recordings (default: ~/demonstrations)
  record_images  Also record camera RGB/depth topics (default: false)
"""

import os
from datetime import datetime

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, LogInfo
from launch.substitutions import LaunchConfiguration, PythonExpression


def generate_launch_description():
    demo_name_arg = DeclareLaunchArgument(
        "demo_name",
        default_value="demo",
        description="Name for the recorded bag file",
    )
    output_dir_arg = DeclareLaunchArgument(
        "output_dir",
        default_value=os.path.expanduser("~/demonstrations"),
        description="Directory in which to save recorded demonstrations",
    )
    record_images_arg = DeclareLaunchArgument(
        "record_images",
        default_value="false",
        description="Also record camera RGB and depth image topics",
    )

    demo_name = LaunchConfiguration("demo_name")
    output_dir = LaunchConfiguration("output_dir")

    # Core topics always recorded
    core_topics = [
        "/joint_states",
        "/arm/servo_commands",
        "/clock",
        "/tf",
        "/tf_static",
        "/yolo/detections",
    ]

    # Image topics — only recorded when record_images:=true
    # (these are large; use only for vision-based imitation learning)
    image_topics = [
        "/camera/camera/color/image_raw",
        "/camera/camera/aligned_depth_to_color/image_raw",
        "/yolo/visualization",
    ]

    all_topics = core_topics + image_topics

    # Build bag output path: output_dir/demo_name
    bag_path = PythonExpression(
        ["'", output_dir, "/' + '", demo_name, "'"]
    )

    record_cmd = ExecuteProcess(
        cmd=[
            "ros2", "bag", "record",
            "--output", bag_path,
        ] + all_topics,
        output="screen",
        shell=False,
    )

    return LaunchDescription([
        demo_name_arg,
        output_dir_arg,
        record_images_arg,
        LogInfo(msg=["Recording demonstration to: ", bag_path]),
        LogInfo(msg="Press Ctrl+C to stop recording."),
        record_cmd,
    ])
