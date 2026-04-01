from moveit_configs_utils import MoveItConfigsBuilder
from moveit_configs_utils.launches import generate_moveit_rviz_launch


def generate_launch_description():
    moveit_config = MoveItConfigsBuilder("robotic_arm_model_v3", package_name="robotic_arm_v3_config").to_moveit_configs()
    return generate_moveit_rviz_launch(moveit_config)
