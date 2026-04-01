#!/usr/bin/env python3
"""
vision.launch.py — Vision pipeline mode

Starts the perception stack for chess piece detection and web monitoring.

Requires a RealSense RGB-D camera. The realsense-ros driver must be
installed (initialize the git submodule in src/external/realsense-ros/).

Two modes (set via the 'mode' launch argument):
  detect  (default) — runs yolo_detector, publishes /yolo/detections
                      and /yolo/visualization
  collect           — runs data_collector instead; saves labelled images
                      to ~/chess_dataset/images/ for training

Both modes start the web interface at http://localhost:5000 for live
monitoring of camera feeds and detection output.

Usage:
  # Detection mode (default)
  ros2 launch arm_bringup vision.launch.py

  # Data collection mode
  ros2 launch arm_bringup vision.launch.py mode:=collect

Launch arguments:
  mode                  'detect' or 'collect' (default: detect)
  confidence_threshold  YOLO detection confidence (default: 0.4)
  frame_skip            Process every Nth frame (default: 2)
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch.conditions import IfCondition
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    mode_arg = DeclareLaunchArgument(
        "mode", default_value="detect",
        description="'detect' to run YOLO inference, 'collect' to save training images"
    )
    confidence_arg = DeclareLaunchArgument(
        "confidence_threshold", default_value="0.4",
        description="YOLO confidence threshold (0.0–1.0)"
    )
    frame_skip_arg = DeclareLaunchArgument(
        "frame_skip", default_value="2",
        description="Process every Nth camera frame"
    )

    is_detect = IfCondition(
        PythonExpression(["'", LaunchConfiguration("mode"), "' == 'detect'"])
    )
    is_collect = IfCondition(
        PythonExpression(["'", LaunchConfiguration("mode"), "' == 'collect'"])
    )

    # ── RealSense camera driver ────────────────────────────────────────────────
    realsense_share = get_package_share_directory("realsense2_camera")
    realsense = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(realsense_share, "launch", "rs_launch.py")
        ),
        launch_arguments={
            "align_depth.enable": "true",
        }.items(),
    )

    # ── YOLO detector (detect mode only) ─────────────────────────────────────
    yolo_detector = Node(
        package="yolov5_detection",
        executable="yolo_detector",
        name="yolo_detector",
        parameters=[{
            "confidence_threshold": LaunchConfiguration("confidence_threshold"),
            "frame_skip": LaunchConfiguration("frame_skip"),
        }],
        output="screen",
        condition=is_detect,
    )

    # ── Data collector (collect mode only) ───────────────────────────────────
    data_collector = Node(
        package="chess_data_collector",
        executable="auto_collect",
        name="data_collector",
        output="screen",
        condition=is_collect,
    )

    # ── Web monitoring dashboard (both modes) ────────────────────────────────
    web_interface = Node(
        package="web_interface",
        executable="flask_server",
        name="web_interface",
        output="screen",
    )

    return LaunchDescription([
        mode_arg,
        confidence_arg,
        frame_skip_arg,
        realsense,
        yolo_detector,
        data_collector,
        web_interface,
    ])
