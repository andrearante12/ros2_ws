#!/usr/bin/env python3
"""
ik_positioning.launch.py — IK Positioning mode

Starts MoveIt2 and pose_printer so the arm can be moved to a specific
Cartesian coordinate via the gradient descent IK solver.

pose_printer runs in 'direct' mode: it owns the serial port and relays
commands published to /arm/servo_commands — preventing conflicts with
any other node trying to write to the same port.

Usage:
  ros2 launch arm_bringup ik_positioning.launch.py

  Then in a separate terminal, send a target position:
    ros2 run move_program move_program <x> <y> <z>
    # Example: ros2 run move_program move_program 0.7 -1.2 1.145

Launch arguments:
  serial_port   Serial port for Arduino (default: /dev/ttyUSB0)
  dry_run       If true, log serial commands instead of sending them
                (default: false). Useful for testing without hardware.

  Example (dry run, no hardware needed):
    ros2 launch arm_bringup ik_positioning.launch.py dry_run:=true
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    serial_port_arg = DeclareLaunchArgument(
        "serial_port", default_value="/dev/ttyUSB0",
        description="Serial port connected to the Arduino Nano"
    )
    dry_run_arg = DeclareLaunchArgument(
        "dry_run", default_value="false",
        description="Log servo commands instead of sending over serial"
    )

    moveit_config_share = get_package_share_directory("robotic_arm_v3_config")

    moveit = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(moveit_config_share, "launch", "demo_with_controllers.launch.py")
        )
    )

    # pose_printer in 'direct' mode — sole owner of the serial port
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
        moveit,
        pose_printer,
    ])
