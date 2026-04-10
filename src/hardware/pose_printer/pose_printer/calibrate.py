#!/usr/bin/env python3
"""
Interactive servo calibration — controls both MuJoCo sim and real servos.

Requires MuJoCo sim running:  ros2 launch arm_bringup mujoco.launch.py

For each servo:
  1. Resets MuJoCo to home (all joints 0°)
  2. User nudges real servo with arrow keys to match
  3. MuJoCo moves that joint to +30°, user confirms direction

Usage:
  ros2 run pose_printer calibrate
  ros2 run pose_printer calibrate --ros-args -p serial_port:=/dev/ttyUSB0
"""

import math
import sys
import termios
import time
import tty

import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from control_msgs.action import FollowJointTrajectory, GripperCommand
from trajectory_msgs.msg import JointTrajectoryPoint
from std_msgs.msg import Float64MultiArray
from builtin_interfaces.msg import Duration
from mujoco_ros2_control_msgs.srv import ResetWorld
import serial


ARM_JOINTS = ['joint_1', 'joint_2', 'joint_3', 'joint_4']

SERVO_JOINTS = [
    ('servo0', 'joint_1'),
    ('servo1', 'joint_2'),
    ('servo2', 'joint_3'),
    ('servo3', 'joint_4'),
    ('servo4', 'end_effector_joint'),
    ('servo5', 'end_effector_top_joint'),
]

CALIBRATION_ANGLE_RAD = 0.5236  # 30 degrees


class CalibrateNode(Node):
    def __init__(self):
        super().__init__('servo_calibrator', parameter_overrides=[
            rclpy.Parameter('use_sim_time', rclpy.Parameter.Type.BOOL, True),
        ])

        self.declare_parameter('serial_port', '/dev/ttyUSB0')
        self.declare_parameter('baud_rate', 115200)

        # Action client for arm joints (joint_1-4)
        self.arm_action = ActionClient(
            self, FollowJointTrajectory,
            '/robotic_arm_controller/follow_joint_trajectory'
        )

        # Action client for end_effector_joint
        self.gripper_action = ActionClient(
            self, GripperCommand,
            '/hand_controller/gripper_cmd'
        )

        # Publisher for finger joints
        self.finger_pub = self.create_publisher(
            Float64MultiArray, '/finger_controller/commands', 10
        )

        # Reset service
        self.reset_client = self.create_client(
            ResetWorld, '/mujoco_ros2_control_node/reset_world'
        )

    def wait_for_services(self):
        print("Waiting for MuJoCo controllers...", end='', flush=True)
        self.arm_action.wait_for_server(timeout_sec=10.0)
        self.reset_client.wait_for_service(timeout_sec=10.0)
        print(" ready.")

    def reset_mujoco(self):
        """Reset all MuJoCo joints to 0."""
        req = ResetWorld.Request()
        future = self.reset_client.call_async(req)
        rclpy.spin_until_future_complete(self, future, timeout_sec=5.0)
        # Also command controllers to 0 so they don't drive back to last target
        self.set_arm_joints([0.0, 0.0, 0.0, 0.0])
        self.set_fingers(0.0, 0.0)
        time.sleep(0.5)

    def set_arm_joints(self, positions):
        """Send arm joints (joint_1-4) to given positions (radians).

        positions: list of 4 floats
        """
        goal = FollowJointTrajectory.Goal()
        goal.trajectory.joint_names = ARM_JOINTS

        point = JointTrajectoryPoint()
        point.positions = list(positions)
        point.time_from_start = Duration(sec=2, nanosec=0)
        goal.trajectory.points = [point]

        future = self.arm_action.send_goal_async(goal)
        rclpy.spin_until_future_complete(self, future, timeout_sec=5.0)
        goal_handle = future.result()
        if goal_handle and goal_handle.accepted:
            result_future = goal_handle.get_result_async()
            rclpy.spin_until_future_complete(self, result_future, timeout_sec=10.0)
        time.sleep(0.5)

    def set_gripper(self, position_rad):
        """Set end_effector_joint via gripper action."""
        goal = GripperCommand.Goal()
        goal.command.position = position_rad
        goal.command.max_effort = 10.0

        future = self.gripper_action.send_goal_async(goal)
        rclpy.spin_until_future_complete(self, future, timeout_sec=5.0)
        goal_handle = future.result()
        if goal_handle and goal_handle.accepted:
            result_future = goal_handle.get_result_async()
            rclpy.spin_until_future_complete(self, result_future, timeout_sec=10.0)
        time.sleep(0.5)

    def set_fingers(self, top_rad, bottom_rad):
        """Set finger joints directly."""
        msg = Float64MultiArray()
        msg.data = [top_rad, bottom_rad]
        self.finger_pub.publish(msg)
        time.sleep(0.5)

    def set_joint(self, joint_name, angle_rad):
        """Set a single joint to angle_rad, others to 0."""
        if joint_name in ARM_JOINTS:
            positions = [0.0] * 4
            idx = ARM_JOINTS.index(joint_name)
            positions[idx] = angle_rad
            self.set_arm_joints(positions)
        elif joint_name == 'end_effector_joint':
            self.set_gripper(angle_rad)
        elif joint_name == 'end_effector_top_joint':
            self.set_fingers(angle_rad, -angle_rad)


def get_key(settings):
    tty.setraw(sys.stdin.fileno())
    try:
        ch = sys.stdin.read(1)
        if ch == '\x1b':
            ch2 = sys.stdin.read(1)
            ch3 = sys.stdin.read(1)
            return {'A': 'up', 'B': 'down', 'C': 'right', 'D': 'left'}.get(ch3, '')
        return ch
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)


def send_servo(ser, servo_name, angle):
    ser.write(f"{servo_name}={angle}\n".encode())
    ser.flush()


def main():
    rclpy.init()
    node = CalibrateNode()

    port = node.get_parameter('serial_port').get_parameter_value().string_value
    baud = node.get_parameter('baud_rate').get_parameter_value().integer_value

    print(f"Opening {port} at {baud} baud...")
    ser = serial.Serial(port, baud, timeout=1)
    time.sleep(2)
    print("Connected to Arduino.")

    node.wait_for_services()

    settings = termios.tcgetattr(sys.stdin)
    results = []

    print()
    print("=" * 60)
    print("SERVO CALIBRATION")
    print("=" * 60)
    print()
    print("Controls:")
    print("  Up/Down    : nudge servo +/- 1°")
    print("  Left/Right : nudge servo +/- 5°")
    print("  Enter      : confirm offset")
    print("  S          : skip servo")
    print("  Ctrl+C     : abort")
    print()

    try:
        for servo_name, joint_name in SERVO_JOINTS:
            print("-" * 60)
            print(f"  {servo_name} ({joint_name})")
            print("-" * 60)

            # Step 1: Reset MuJoCo to home (all joints 0)
            print("  Resetting MuJoCo to home...", end='', flush=True)
            node.reset_mujoco()
            # Also reset previously calibrated real servos to their offsets
            for prev_servo, prev_joint, prev_dir, prev_offset in results:
                if prev_dir is not None:
                    send_servo(ser, prev_servo, prev_offset)
            print(" done.")
            print()
            print("  Match real servo to MuJoCo's 0° pose:")

            # Step 2: User nudges real servo to match
            angle = 90
            send_servo(ser, servo_name, angle)

            key = None
            while True:
                sys.stdout.write(f"\r  {servo_name} = {angle:>4}°   (arrows to nudge, Enter to confirm)  ")
                sys.stdout.flush()

                key = get_key(settings)

                if key == 'up':
                    angle += 1
                elif key == 'down':
                    angle -= 1
                elif key == 'right':
                    angle += 5
                elif key == 'left':
                    angle -= 5
                elif key == '\r':
                    break
                elif key == 's':
                    print(f"\n  Skipped.")
                    results.append((servo_name, joint_name, None, None))
                    break
                elif key == '\x03':
                    raise KeyboardInterrupt
                else:
                    continue

                angle = max(0, min(180, angle))
                send_servo(ser, servo_name, angle)

            if key == 's':
                print()
                continue

            offset = angle
            print(f"\n  Offset = {offset}°")

            # Step 3: Move MuJoCo to +30°, check direction
            print()
            print(f"  Moving MuJoCo {joint_name} to +30°...", end='', flush=True)
            node.set_joint(joint_name, CALIBRATION_ANGLE_RAD)
            print(" done.")

            # Also move real servo in the positive direction to show the user
            test_angle = offset + 30
            test_angle = max(0, min(180, test_angle))
            send_servo(ser, servo_name, test_angle)

            print(f"  Real servo moved to {test_angle}° (offset + 30).")
            print()
            print(f"  Did the real arm move the SAME direction as MuJoCo? (y/n) ", end='', flush=True)

            direction = None
            while True:
                dk = get_key(settings)
                if dk in ('y', 'Y'):
                    direction = +1
                    print("yes -> direction = +1")
                    break
                elif dk in ('n', 'N'):
                    direction = -1
                    print("no  -> direction = -1")
                    break
                elif dk == '\x03':
                    raise KeyboardInterrupt

            # Step 4: Verify — apply mapping, let user fine-tune offset
            verify_angle = round(direction * 30 + offset)
            verify_angle = max(0, min(180, verify_angle))
            send_servo(ser, servo_name, verify_angle)
            print()
            print(f"  Verification: {direction} * 30 + {offset} = {verify_angle}°")
            print(f"  Nudge offset with arrows if needed, Enter to confirm:")

            while True:
                computed = round(direction * 30 + offset)
                computed = max(0, min(180, computed))
                send_servo(ser, servo_name, computed)
                sys.stdout.write(f"\r  offset = {offset:>4}°  →  servo = {computed:>4}°   (arrows to adjust, Enter to confirm)  ")
                sys.stdout.flush()

                vk = get_key(settings)
                if vk == 'up':
                    offset += 1
                elif vk == 'down':
                    offset -= 1
                elif vk == 'right':
                    offset += 5
                elif vk == 'left':
                    offset -= 5
                elif vk == '\r':
                    break
                elif vk == '\x03':
                    raise KeyboardInterrupt
            print()

            results.append((servo_name, joint_name, direction, offset))
            print()

    except KeyboardInterrupt:
        print("\n\nAborted.")
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)

    if results:
        print()
        print("=" * 60)
        print("RESULTS — paste into pose_printer_node.py")
        print("=" * 60)
        print()
        print("# (servo_name, direction, offset, clamp_range)")
        print("# final_angle = direction * mujoco_degrees + offset")
        print("JOINT_SERVO_MAP = {")
        for servo_name, joint_name, direction, offset in results:
            if direction is None:
                print(f"    '{joint_name}': ('{servo_name}', +1, 0, None),  # SKIPPED")
            else:
                dir_str = f"+{direction}" if direction > 0 else str(direction)
                pad = max(0, 25 - len(joint_name))
                print(f"    '{joint_name}':{' ' * pad} ('{servo_name}', {dir_str}, {offset:>4}, None),")
        print("}")

    ser.close()
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
