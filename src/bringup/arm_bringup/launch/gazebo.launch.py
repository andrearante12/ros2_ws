#!/usr/bin/env python3
"""
gazebo.launch.py — Gazebo Harmonic physics simulation

Runs the full arm simulation with real physics, including:
  - Gazebo Harmonic (gz-sim) with the arm world (table + target object)
  - Robot spawned from URDF with gz_ros2_control/GazeboSimSystem hardware interface
  - ros2_control controllers (JointTrajectoryController + GripperActionController)
  - MoveIt2 motion planning (move_group) with use_sim_time=true
  - RViz2 visualisation
  - ros_gz_bridge for clock synchronisation

This launch file is the foundation for:
  - Imitation learning demonstration collection
  - MoveIt trajectory planning with physics validation
  - Reinforcement learning environment via ros2_control interfaces

Usage:
  ros2 launch arm_bringup gazebo.launch.py

  To also record a demonstration simultaneously:
    ros2 launch arm_bringup record_demo.launch.py  (in a second terminal)

  To drive with the wearable controller:
    ros2 launch arm_bringup teleop.launch.py backend:=gazebo

Launch arguments:
  world        Path to the Gazebo world SDF (default: arm_world.sdf)
  gz_headless  Run Gazebo without GUI, default false
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    RegisterEventHandler,
    AppendEnvironmentVariable,
)
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import (
    LaunchConfiguration,
    PathJoinSubstitution,
    Command,
    FindExecutable,
)
from launch.conditions import IfCondition
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    pkg_config = get_package_share_directory("robotic_arm_v3_config")
    pkg_model = get_package_share_directory("robotic_arm_model_v3")

    # ── Launch arguments ──────────────────────────────────────────────────────
    world_arg = DeclareLaunchArgument(
        "world",
        default_value=os.path.join(pkg_config, "worlds", "arm_world.sdf"),
        description="Path to Gazebo world SDF file",
    )
    headless_arg = DeclareLaunchArgument(
        "gz_headless",
        default_value="false",
        description="Run Gazebo without GUI (server only)",
    )

    world = LaunchConfiguration("world")
    headless = LaunchConfiguration("gz_headless")

    # ── Environment: help Gazebo find package:// mesh URIs ───────────────────
    gz_resource_path = AppendEnvironmentVariable(
        "GZ_SIM_RESOURCE_PATH",
        os.path.join(pkg_model, ".."),  # parent of robotic_arm_model_v3 share dir
    )

    # ── Robot description (xacro with use_sim:=true) ─────────────────────────
    # MoveItConfigsBuilder's .setup_assistant points to the base URDF which has
    # no <gazebo> plugin block. We must generate robot_description ourselves via
    # xacro so that the gz_ros2_control plugin is included in what gets published
    # to /robot_description and passed to ros_gz_sim create.
    robot_description_content = Command([
        FindExecutable(name="xacro"), " ",
        PathJoinSubstitution([
            FindPackageShare("robotic_arm_v3_config"),
            "config", "robotic_arm_model_v3.urdf.xacro",
        ]),
        " use_sim:=true",
    ])
    robot_description = {"robot_description": robot_description_content}

    moveit_config = (
        MoveItConfigsBuilder("robotic_arm_v3", package_name="robotic_arm_v3_config")
        .robot_description(mappings={"use_sim": "true"})
        .robot_description_kinematics()
        .trajectory_execution(
            file_path=os.path.join(pkg_config, "config", "moveit_controllers.yaml")
        )
        .planning_scene_monitor(
            publish_robot_description=True,
            publish_robot_description_semantic=True,
        )
        .planning_pipelines(pipelines=["ompl"])
        .to_moveit_configs()
    )

    # ── Gazebo simulator ──────────────────────────────────────────────────────
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory("ros_gz_sim"), "launch", "gz_sim.launch.py"
            )
        ),
        launch_arguments={
            "gz_args": ["-v 4 -r ", world],
        }.items(),
    )

    # ── Robot state publisher ─────────────────────────────────────────────────
    # Use the xacro-generated description (includes <gazebo> plugin block),
    # NOT moveit_config.robot_description (which comes from the base URDF via
    # .setup_assistant and lacks the plugin).
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[
            robot_description,
            {"use_sim_time": True},
        ],
        output="screen",
    )

    # ── Static TF: world → base_link ─────────────────────────────────────────
    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        arguments=["--frame-id", "world", "--child-frame-id", "base_link"],
        parameters=[{"use_sim_time": True}],
    )

    # ── Spawn robot from robot_description topic ──────────────────────────────
    # ros_gz_sim create reads /robot_description, converts URDF→SDF, spawns model.
    # The <gazebo> plugin tag in the xacro becomes a Gazebo system plugin.
    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        arguments=[
            "-topic", "robot_description",
            "-name", "robotic_arm",
            "-allow_renaming", "true",
        ],
        parameters=[{"use_sim_time": True}],
        output="screen",
    )

    # ── Clock bridge (Gazebo → ROS) ───────────────────────────────────────────
    clock_bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        arguments=["/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock"],
        parameters=[{"use_sim_time": True}],
        output="screen",
    )

    # ── Controller spawning (sequenced via event handlers) ────────────────────
    # Must wait for the robot to be fully spawned before controllers can connect.
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
        parameters=[{"use_sim_time": True}],
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["robotic_arm_controller"],
        parameters=[{"use_sim_time": True}],
    )

    hand_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["hand_controller"],
        parameters=[{"use_sim_time": True}],
    )

    # Spawn joint_state_broadcaster only after robot is loaded
    spawn_jsb_after_robot = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=spawn_robot,
            on_exit=[joint_state_broadcaster_spawner],
        )
    )
    # Spawn arm controller after joint state broadcaster is up
    spawn_arm_after_jsb = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=joint_state_broadcaster_spawner,
            on_exit=[arm_controller_spawner, hand_controller_spawner],
        )
    )

    # ── MoveIt move_group ─────────────────────────────────────────────────────
    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        parameters=[
            moveit_config.to_dict(),
            moveit_config.robot_description_kinematics,
            {"use_sim_time": True},
        ],
        output="screen",
    )

    # ── RViz ─────────────────────────────────────────────────────────────────
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", os.path.join(pkg_config, "config", "moveit.rviz")],
        parameters=[
            robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.planning_pipelines,
            moveit_config.joint_limits,
            {"use_sim_time": True},
        ],
        output="log",
    )

    return LaunchDescription([
        gz_resource_path,
        world_arg,
        headless_arg,
        gazebo,
        robot_state_publisher,
        static_tf,
        clock_bridge,
        spawn_robot,
        spawn_jsb_after_robot,
        spawn_arm_after_jsb,
        move_group,
        rviz,
    ])
