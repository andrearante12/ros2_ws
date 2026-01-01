#!/usr/bin/env python3

import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():

    pkg_name = "robotic_arm_v3_config"
    robot_name = "robotic_arm_v3"   # MUST match URDF/SRDF root

    moveit_config_share = get_package_share_directory(pkg_name)

    rviz_config_arg = DeclareLaunchArgument(
        "rviz_config",
        default_value=PathJoinSubstitution([
            moveit_config_share,
            "config",
            "moveit.rviz"
        ]),
        description="RViz config file"
    )

    rviz_config = LaunchConfiguration("rviz_config")

    moveit_config = (
        MoveItConfigsBuilder(
            robot_name,
            package_name=pkg_name
        )
        .robot_description()
        .robot_description_kinematics()
        .trajectory_execution(
            file_path=os.path.join(
                moveit_config_share,
                "config",
                "moveit_controllers.yaml"
            )
        )
        .planning_scene_monitor(
            publish_robot_description=True,
            publish_robot_description_semantic=True
        )
        .planning_pipelines(pipelines=["ompl"])
        .to_moveit_configs()
    )
    
    

    # --- Core publishers ---
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[moveit_config.robot_description],
        output="screen",
    )



    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        arguments=["0", "0", "0", "0", "0", "0", "world", "base_link"],
    )

    # --- ros2_control ---
    ros2_control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            moveit_config.robot_description,
            os.path.join(moveit_config_share, "config", "ros2_controllers.yaml"),
        ],
        output="screen",
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
        output="screen",
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["robotic_arm_controller"],
        output="screen",
    )

    # --- MoveIt ---
    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        parameters=[moveit_config.to_dict(), moveit_config.robot_description_kinematics],
        output="screen",
    )
    
    # Add this NEW node
    move_program_node = Node(
        package="move_program",
        executable="move_program",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
        ],
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_config],
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.planning_pipelines,
            moveit_config.joint_limits,
        ],
        output="log",
    )

    return LaunchDescription([
        rviz_config_arg,
        static_tf,
        robot_state_publisher,
        ros2_control_node,
        joint_state_broadcaster_spawner,
        arm_controller_spawner,
        move_group,
        move_program_node,
        rviz_node,
    ])