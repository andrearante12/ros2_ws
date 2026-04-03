#!/usr/bin/env python3
"""
esp32_viz.launch.py — ESP32 wearable controller visualization

Starts the full pipeline for visualizing the ESP32 controller input in RViz:
  1. mqtt_imu_node   — bridges ESP32 MQTT sensor data → /odom + /imu/data
  2. esp32_viz_node   — maps OTOS input to workspace coordinates, publishes
                        RViz markers (target sphere, orientation arrow, bounding box)

Open RViz separately, set Fixed Frame to 'base_link', and add a MarkerArray
display subscribed to '/esp32_viz/markers'.

Usage:
  ros2 launch arm_bringup esp32_viz.launch.py
"""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    mqtt_imu = Node(
        package="mqtt_imu",
        executable="mqtt_imu_node",
        name="mqtt_imu_node",
        output="screen",
    )

    esp32_viz = Node(
        package="esp32_viz",
        executable="esp32_viz_node",
        name="esp32_viz",
        output="screen",
    )

    return LaunchDescription([
        mqtt_imu,
        esp32_viz,
    ])
