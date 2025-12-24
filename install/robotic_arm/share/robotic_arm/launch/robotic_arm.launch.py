import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import Command, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

    # Package paths
    pkg_share = get_package_share_directory('robotic_arm')

    default_urdf_path = os.path.join(
        pkg_share,
        'urdf',
        'robotic_arm.urdf'
    )

    default_rviz_config_path = os.path.join(
        pkg_share,
        'rviz',
        'rviz_basic_settings.rviz'
    )

    # Launch arguments
    gui = LaunchConfiguration('gui')
    urdf_model = LaunchConfiguration('urdf_model')
    use_rviz = LaunchConfiguration('use_rviz')
    use_sim_time = LaunchConfiguration('use_sim_time')

    return LaunchDescription([

        # --------------------
        # Launch arguments
        # --------------------
        DeclareLaunchArgument(
            'urdf_model',
            default_value=default_urdf_path,
            description='Absolute path to robot URDF file'
        ),

        DeclareLaunchArgument(
            'gui',
            default_value='True',
            description='Launch joint_state_publisher_gui'
        ),

        DeclareLaunchArgument(
            'use_rviz',
            default_value='True',
            description='Launch RViz'
        ),

        DeclareLaunchArgument(
            'use_sim_time',
            default_value='False',
            description='Use simulation time'
        ),

        # --------------------
        # Joint State Publisher (no GUI)
        # --------------------
        Node(
            condition=UnlessCondition(gui),
            package='joint_state_publisher',
            executable='joint_state_publisher',
            name='joint_state_publisher'
        ),

        # --------------------
        # Joint State Publisher GUI
        # --------------------
        Node(
            condition=IfCondition(gui),
            package='joint_state_publisher_gui',
            executable='joint_state_publisher_gui',
            name='joint_state_publisher_gui'
        ),

        # --------------------
        # Robot State Publisher
        # --------------------
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            parameters=[{
                'use_sim_time': use_sim_time,
                'robot_description': ParameterValue(
                    Command(['cat ', urdf_model]),
                    value_type=str
                )
            }]
        ),

        # --------------------
        # RViz
        # --------------------
        Node(
            condition=IfCondition(use_rviz),
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-d', default_rviz_config_path]
        )
    ])
