#!/usr/bin/env python3
"""
cube_detect.launch.py — RealSense + cube position detector

Starts the RealSense camera, a static TF for the camera mount, and
the color-segmentation cube detector that publishes /object/position.

The camera mount transform must be measured once and passed as arguments.
For a top-down mount, cam_pitch is typically -pi/2 (-1.5708).

Usage:
  ros2 launch arm_bringup cube_detect.launch.py cam_x:=0.0 cam_y:=0.15 cam_z:=0.5 cam_pitch:=-1.5708
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    # Camera mount transform arguments
    cam_x = DeclareLaunchArgument('cam_x', default_value='0.0')
    cam_y = DeclareLaunchArgument('cam_y', default_value='0.15')
    cam_z = DeclareLaunchArgument('cam_z', default_value='0.5')
    cam_roll = DeclareLaunchArgument('cam_roll', default_value='0.0')
    cam_pitch = DeclareLaunchArgument('cam_pitch', default_value='-1.5708')
    cam_yaw = DeclareLaunchArgument('cam_yaw', default_value='0.0')

    # Detection parameters
    v_max_arg = DeclareLaunchArgument('v_max', default_value='50')
    s_max_arg = DeclareLaunchArgument('s_max', default_value='80')
    publish_debug_arg = DeclareLaunchArgument('publish_debug', default_value='false')

    # RealSense camera
    realsense_share = get_package_share_directory('realsense2_camera')
    realsense = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(realsense_share, 'launch', 'rs_launch.py')
        ),
        launch_arguments={'align_depth.enable': 'true'}.items(),
    )

    # Static TF: base_link → camera_link
    static_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='camera_static_tf',
        arguments=[
            '--x', LaunchConfiguration('cam_x'),
            '--y', LaunchConfiguration('cam_y'),
            '--z', LaunchConfiguration('cam_z'),
            '--roll', LaunchConfiguration('cam_roll'),
            '--pitch', LaunchConfiguration('cam_pitch'),
            '--yaw', LaunchConfiguration('cam_yaw'),
            '--frame-id', 'base_link',
            '--child-frame-id', 'camera_link',
        ],
    )

    # Cube detector
    detector = Node(
        package='cube_detector',
        executable='cube_detector',
        name='cube_detector',
        parameters=[{
            'v_max': LaunchConfiguration('v_max'),
            's_max': LaunchConfiguration('s_max'),
            'publish_debug': LaunchConfiguration('publish_debug'),
        }],
        output='screen',
    )

    return LaunchDescription([
        cam_x, cam_y, cam_z, cam_roll, cam_pitch, cam_yaw,
        v_max_arg, s_max_arg, publish_debug_arg,
        realsense,
        static_tf,
        detector,
    ])
