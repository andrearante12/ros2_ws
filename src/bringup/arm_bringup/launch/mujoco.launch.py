#!/usr/bin/env python3
"""
mujoco.launch.py — MuJoCo physics simulation

Runs the full arm simulation with real physics, including:
  - MuJoCo simulator (embedded inside the ros2_control_node)
  - ros2_control controllers (JointTrajectoryController + GripperActionController)
  - MoveIt2 motion planning (move_group) with wall-clock time
  - RViz2 visualisation

Key differences from Gazebo:
  - No separate simulator process — MuJoCo is embedded in ros2_control_node
  - No clock bridge — mujoco_ros2_control publishes /clock natively
  - No spawn_robot — robot is loaded from robot_description parameter directly
  - MuJoCo Simulate GUI opens automatically (set headless:=true to disable)

Usage:
  ros2 launch arm_bringup mujoco.launch.py
  ros2 launch arm_bringup mujoco.launch.py headless:=true

  To record a demonstration:
    ros2 launch arm_bringup record_demo.launch.py  (in a second terminal)

  To drive with the wearable controller:
    ros2 launch arm_bringup teleop.launch.py backend:=mujoco

Launch arguments:
  headless   Run MuJoCo without GUI (default: false)
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import (
    LaunchConfiguration,
    PathJoinSubstitution,
    Command,
    FindExecutable,
)
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    pkg_config = get_package_share_directory("robotic_arm_v3_config")

    # ── Launch arguments ──────────────────────────────────────────────────────
    headless_arg = DeclareLaunchArgument(
        "headless",
        default_value="false",
        description="Run MuJoCo without GUI (headless server mode)",
    )

    headless = LaunchConfiguration("headless")

    # ── Robot description (xacro with use_sim:=mujoco) ───────────────────────
    # Generates URDF with mujoco_ros2_control/MujocoSystemInterface as the
    # hardware plugin. The plugin reads arm_scene.xml for the MuJoCo environment.
    robot_description_content = Command([
        FindExecutable(name="xacro"), " ",
        PathJoinSubstitution([
            FindPackageShare("robotic_arm_v3_config"),
            "config", "robotic_arm_model_v3.urdf.xacro",
        ]),
        " use_sim:=mujoco headless:=", headless,
    ])
    robot_description = {"robot_description": robot_description_content}

    moveit_config = (
        MoveItConfigsBuilder("robotic_arm_v3", package_name="robotic_arm_v3_config")
        .robot_description(mappings={"use_sim": "mujoco"})
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

    # ── Robot state publisher ─────────────────────────────────────────────────
    # use_sim_time=True: mujoco_ros2_control publishes /clock starting at t=0;
    # all nodes must use sim time so TF timestamps stay monotonically increasing.
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[
            robot_description,
            {"use_sim_time": True},
        ],
        output="screen",
    )

    # ── MuJoCo ros2_control node ──────────────────────────────────────────────
    # This node embeds the MuJoCo physics simulation AND acts as the
    # controller manager. It publishes /clock and opens the Simulate GUI
    # (unless headless=true). No separate simulator process is needed.
    mujoco_node = Node(
        package="mujoco_ros2_control",
        executable="ros2_control_node",
        parameters=[
            robot_description,
            {"use_sim_time": True},
            os.path.join(pkg_config, "config", "ros2_controllers.yaml"),
        ],
        output="screen",
    )

    # ── Controller spawning ───────────────────────────────────────────────────
    # No OnProcessExit sequencing needed — the mujoco_node IS the controller
    # manager, so spawners just retry until it's ready.
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager-timeout", "30"],
        parameters=[{"use_sim_time": True}],
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["robotic_arm_controller", "--controller-manager-timeout", "30"],
        parameters=[{"use_sim_time": True}],
    )

    hand_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["hand_controller", "--controller-manager-timeout", "30"],
        parameters=[{"use_sim_time": True}],
    )

    finger_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["finger_controller", "--controller-manager-timeout", "30"],
        parameters=[{"use_sim_time": True}],
    )

    # ── MoveIt move_group ─────────────────────────────────────────────────────
    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        parameters=[
            moveit_config.to_dict(),
            moveit_config.robot_description_kinematics,
            robot_description,
            {"use_sim_time": True},
        ],
        output="screen",
    )

    # ── Gripper control node ──────────────────────────────────────────────────
    gripper_control = Node(
        package="moveit_controls",
        executable="gripper_control",
        parameters=[{"use_sim_time": True}],
        output="screen",
    )

    return LaunchDescription([
        headless_arg,
        robot_state_publisher,
        mujoco_node,
        joint_state_broadcaster_spawner,
        arm_controller_spawner,
        hand_controller_spawner,
        finger_controller_spawner,
        move_group,
        gripper_control,
    ])
