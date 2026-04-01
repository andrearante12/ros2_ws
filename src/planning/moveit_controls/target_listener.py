#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Point
from geometry_msgs.msg import PoseStamped
from moveit_ros_planning_interface import MoveGroupInterface


class TargetListener(Node):
    def __init__(self):
        super().__init__('target_listener')

        self.move_group = MoveGroupInterface(
            node=self,
            joint_model_group='arm'
        )

        self.create_subscription(
            Point,
            '/target_xyz',
            self.cb,
            10
        )

        self.get_logger().info("Listening on /target_xyz")

    def cb(self, msg):
        pose = PoseStamped()
        pose.header.frame_id = "base_link"
        pose.pose.position = msg
        pose.pose.orientation.w = 1.0

        self.move_group.set_pose_target(pose)
        self.move_group.plan_and_execute()


def main():
    rclpy.init()
    node = TargetListener()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
