#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from moveit_ros_planning_interface import MoveGroupInterface


class MoveToXYZ(Node):
    def __init__(self):
        super().__init__('move_to_xyz')

        self.move_group = MoveGroupInterface(
            node=self,
            joint_model_group='arm'
        )

        self.get_logger().info("MoveToXYZ ready")

    def move(self, x, y, z):
        pose = PoseStamped()
        pose.header.frame_id = "base_link"
        pose.pose.position.x = x
        pose.pose.position.y = y
        pose.pose.position.z = z
        pose.pose.orientation.w = 1.0

        self.move_group.set_pose_target(pose)
        self.move_group.plan_and_execute()


def main():
    rclpy.init()
    node = MoveToXYZ()

    # example target
    node.move(0.15, 0.0, 0.12)

    rclpy.shutdown()


if __name__ == "__main__":
    main()
