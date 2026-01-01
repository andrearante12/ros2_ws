#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
import math
import re
import serial
import time


class PosePrinter(Node):
    def __init__(self):
        super().__init__('pose_printer')

        # ===== Serial setup =====
        self.serial_port = '/dev/ttyUSB0'  # change if needed
        self.baud_rate = 9600

        try:
            self.ser = serial.Serial(self.serial_port, self.baud_rate, timeout=1)
            time.sleep(2)  # allow Arduino reset
            self.get_logger().info(f"Connected to {self.serial_port}")
        except serial.SerialException as e:
            self.get_logger().error(f"Serial error: {e}")
            self.ser = None

        # ===== ROS setup =====
        self.latest_joint_state = None

        self.subscription = self.create_subscription(
            JointState,
            '/joint_states',
            self.joint_state_callback,
            10
        )

        # 0.5 Hz timer
        self.timer = self.create_timer(1, self.timer_callback)


    def joint_state_callback(self, msg: JointState):
        self.latest_joint_state = msg

    def timer_callback(self):
        if self.latest_joint_state is None or self.ser is None:
            return

        joint_positions = dict(
            zip(self.latest_joint_state.name, self.latest_joint_state.position)
        )

        arm_joints = {
            k: v for k, v in joint_positions.items()
            if k.startswith("joint_")
        }

        # Send in deterministic order
        for name, pos in sorted(
            arm_joints.items(),
            key=lambda item: int(item[0].split('_')[-1])
        ):
            servo = self.joint_to_servo(name)
            angle = round(math.degrees(pos))

            command = f"{servo}={angle}\n"
            self.ser.write(command.encode())
            self.ser.flush()

    def joint_to_servo(self, joint_name: str) -> str:
        match = re.search(r'\d+', joint_name)
        if not match:
            return joint_name
        return f"servo{int(match.group()) - 1}"

    def destroy_node(self):
        if self.ser:
            self.ser.close()
        super().destroy_node()


def main():
    rclpy.init()
    node = PosePrinter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
