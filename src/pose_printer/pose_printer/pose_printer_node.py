#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
import math


class PosePrinter(Node):
    def __init__(self):
        super().__init__('pose_printer')

        self.subscription = self.create_subscription(
            JointState,
            '/joint_states',
            self.joint_state_callback,
            10
        )

        self.get_logger().info("Pose Printer started. Listening to /joint_states")

    def joint_state_callback(self, msg: JointState):
        joint_positions = dict(zip(msg.name, msg.position))

        # Print only arm joints
        arm_joints = {k: v for k, v in joint_positions.items() if k.startswith("joint_")}

        timestamp = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9

        # Convert radians → degrees
        output = [
            f"{name}: {math.degrees(pos):.2f}°"
            for name, pos in arm_joints.items()
        ]

        self.get_logger().info(
            f"t={timestamp:.3f} | " + ", ".join(output)
        )


def main():
    rclpy.init()
    node = PosePrinter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
