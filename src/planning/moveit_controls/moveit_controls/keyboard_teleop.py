#!/usr/bin/env python3
"""
Keyboard teleop for arm target position.

Publishes geometry_msgs/Point to /move_to (same interface as esp32_controller),
so it's a drop-in replacement for testing without the wearable.

Also publishes an RViz marker sphere at the target position.

Controls:
  W/S  — Y forward/backward
  A/D  — X left/right
  Q/E  — Z up/down
  R    — Reset to center
  Ctrl+C — Quit

Usage:
  ros2 run moveit_controls keyboard_teleop
  ros2 run moveit_controls keyboard_teleop --ros-args -p step_size:=0.02
"""

import sys
import termios
import tty

import math

import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from control_msgs.action import FollowJointTrajectory, GripperCommand
from geometry_msgs.msg import Point
from sensor_msgs.msg import JointState
from std_msgs.msg import String, Float64MultiArray
from trajectory_msgs.msg import JointTrajectoryPoint
from visualization_msgs.msg import Marker, MarkerArray
from builtin_interfaces.msg import Duration


# Workspace bounds (from CLAUDE.md)
WS_X_MIN, WS_X_MAX = -0.08, 0.09
WS_Y_MIN, WS_Y_MAX = -0.18, 0.18
WS_Z_MIN, WS_Z_MAX = 0.05, 0.25

JOINT_SERVO_NAMES = [
    ('joint_1', 'S0'),
    ('joint_2', 'S1'),
    ('joint_3', 'S2'),
    ('joint_4', 'S3'),
    ('end_effector_joint', 'S4'),
    ('end_effector_top_joint', 'S5'),
]

ARM_JOINTS = ['joint_1', 'joint_2', 'joint_3', 'joint_4']
JOINT_STEP_RAD = 0.05  # ~3 degrees per keypress

GRIPPER_OPEN = [-0.35, 0.35]
GRIPPER_CLOSE = [0.09, -0.09]

HELP_TEXT = """
Keyboard Teleop — Arm Target Control
-------------------------------------
  W/S : Y forward / backward
  A/D : X left / right
  Q/E : Z up / down
  O/C : Open / Close gripper
  R   : Reset to center

  Joint mode (direct joint control):
  1-5 : Select joint (1=base .. 4=wrist, 5=hand rotation)
  0   : Deselect joint (back to XYZ mode)
  When a joint is selected, W/S nudge it

  Ctrl+C : Quit
-------------------------------------
"""


class KeyboardTeleop(Node):
    def __init__(self):
        super().__init__('keyboard_teleop')

        self.declare_parameter('step_size', 0.01)
        self.step = self.get_parameter('step_size').value

        # Start at workspace center
        self.x = (WS_X_MIN + WS_X_MAX) / 2.0
        self.y = (WS_Y_MIN + WS_Y_MAX) / 2.0
        self.z = (WS_Z_MIN + WS_Z_MAX) / 2.0

        self.move_pub = self.create_publisher(Point, '/move_to', 10)
        self.finger_pub = self.create_publisher(Float64MultiArray, '/finger_controller/commands', 10)
        self.servo_cmd_pub = self.create_publisher(String, '/arm/servo_commands', 10)
        self.marker_pub = self.create_publisher(MarkerArray, '/esp32_viz/markers', 10)

        self.arm_action = ActionClient(
            self, FollowJointTrajectory,
            '/robotic_arm_controller/follow_joint_trajectory'
        )
        self.hand_action = ActionClient(
            self, GripperCommand,
            '/hand_controller/gripper_cmd'
        )

        self.servo_values = {}
        self.joint_positions = [0.0] * 4  # current arm joint positions (rad)
        self.ee_joint_pos = 0.0  # end_effector_joint position (rad)
        self.selected_joint = None  # None = XYZ mode, 0-4 = joint index
        self.create_subscription(JointState, '/joint_states', self._joint_state_cb, 10)

        self.get_logger().info(f'Step size: {self.step}m')
        self.get_logger().info(f'Starting at [{self.x:.3f}, {self.y:.3f}, {self.z:.3f}]')

    def publish(self):
        # Clamp to workspace
        self.x = max(WS_X_MIN, min(WS_X_MAX, self.x))
        self.y = max(WS_Y_MIN, min(WS_Y_MAX, self.y))
        self.z = max(WS_Z_MIN, min(WS_Z_MAX, self.z))

        # Publish target point
        msg = Point(x=self.x, y=self.y, z=self.z)
        self.move_pub.publish(msg)

        # Publish RViz marker
        markers = MarkerArray()
        sphere = Marker()
        sphere.header.stamp = self.get_clock().now().to_msg()
        sphere.header.frame_id = 'base_link'
        sphere.ns = 'keyboard_target'
        sphere.id = 0
        sphere.type = Marker.SPHERE
        sphere.action = Marker.ADD
        sphere.pose.position.x = self.x
        sphere.pose.position.y = self.y
        sphere.pose.position.z = self.z
        sphere.pose.orientation.w = 1.0
        sphere.scale.x = 0.04
        sphere.scale.y = 0.04
        sphere.scale.z = 0.04
        sphere.color.r = 0.2
        sphere.color.g = 1.0
        sphere.color.b = 0.2
        sphere.color.a = 0.9
        sphere.lifetime = Duration(sec=2, nanosec=0)
        markers.markers.append(sphere)
        self.marker_pub.publish(markers)

    def _joint_state_cb(self, msg: JointState):
        positions = dict(zip(msg.name, msg.position))
        for joint_name, label in JOINT_SERVO_NAMES:
            if joint_name in positions:
                self.servo_values[label] = round(math.degrees(positions[joint_name]))
        for i, jn in enumerate(ARM_JOINTS):
            if jn in positions:
                self.joint_positions[i] = positions[jn]
        if 'end_effector_joint' in positions:
            self.ee_joint_pos = positions['end_effector_joint']

    def send_arm_joints(self, positions):
        """Send arm joint positions (radians) via trajectory action."""
        goal = FollowJointTrajectory.Goal()
        goal.trajectory.joint_names = ARM_JOINTS
        point = JointTrajectoryPoint()
        point.positions = list(positions)
        point.time_from_start = Duration(sec=0, nanosec=500000000)  # 0.5s
        goal.trajectory.points = [point]
        self.arm_action.send_goal_async(goal)

    def send_ee_joint(self, position):
        """Send end_effector_joint position via gripper action."""
        goal = GripperCommand.Goal()
        goal.command.position = position
        goal.command.max_effort = 10.0
        self.hand_action.send_goal_async(goal)

    def send_gripper(self, values):
        """Send finger positions directly."""
        msg = Float64MultiArray()
        msg.data = values
        self.finger_pub.publish(msg)

    def servo_display(self) -> str:
        if not self.servo_values:
            return ""
        return "  ".join(f"{k}={self.servo_values[k]:>4}°" for _, k in JOINT_SERVO_NAMES if k in self.servo_values)

    def reset(self):
        self.x = (WS_X_MIN + WS_X_MAX) / 2.0
        self.y = (WS_Y_MIN + WS_Y_MAX) / 2.0
        self.z = (WS_Z_MIN + WS_Z_MAX) / 2.0


def get_key(settings):
    tty.setraw(sys.stdin.fileno())
    try:
        key = sys.stdin.read(1)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key


def main(args=None):
    rclpy.init(args=args)
    node = KeyboardTeleop()

    settings = termios.tcgetattr(sys.stdin)
    print(HELP_TEXT)
    print('\n')  # reserve two lines for target + servos

    try:
        while rclpy.ok():
            key = get_key(settings)
            moved = True

            # Joint selection
            if key in ('1', '2', '3', '4', '5'):
                node.selected_joint = int(key) - 1
                moved = False
            elif key == '0':
                node.selected_joint = None
                moved = False
            # Movement: W/S either move Y or nudge selected joint
            elif key == 'w':
                if node.selected_joint is not None:
                    if node.selected_joint < 4:
                        node.joint_positions[node.selected_joint] += JOINT_STEP_RAD
                        node.send_arm_joints(node.joint_positions)
                    else:
                        node.ee_joint_pos += JOINT_STEP_RAD
                        node.send_ee_joint(node.ee_joint_pos)
                else:
                    node.y += node.step
            elif key == 's':
                if node.selected_joint is not None:
                    if node.selected_joint < 4:
                        node.joint_positions[node.selected_joint] -= JOINT_STEP_RAD
                        node.send_arm_joints(node.joint_positions)
                    else:
                        node.ee_joint_pos -= JOINT_STEP_RAD
                        node.send_ee_joint(node.ee_joint_pos)
                else:
                    node.y -= node.step
            elif key == 'a':
                node.x -= node.step
            elif key == 'd':
                node.x += node.step
            elif key == 'q':
                node.z += node.step
            elif key == 'e':
                node.z -= node.step
            elif key == 'o':
                node.send_gripper(GRIPPER_OPEN)
                node.servo_cmd_pub.publish(String(data='servo5=15'))
                moved = False
            elif key == 'c':
                node.send_gripper(GRIPPER_CLOSE)
                node.servo_cmd_pub.publish(String(data='servo5=105'))
                moved = False
            elif key == 'r':
                node.reset()
                node.selected_joint = None
            elif key == '\x03':  # Ctrl+C
                break
            else:
                moved = False

            if moved and node.selected_joint is None:
                node.publish()

            # Process any pending callbacks (updates servo_values)
            rclpy.spin_once(node, timeout_sec=0)

            if node.selected_joint is not None:
                if node.selected_joint < 4:
                    jname = ARM_JOINTS[node.selected_joint]
                    jdeg = round(math.degrees(node.joint_positions[node.selected_joint]))
                else:
                    jname = 'end_effector_joint'
                    jdeg = round(math.degrees(node.ee_joint_pos))
                mode_str = f'Joint: {jname} = {jdeg}°  (W/S to nudge, 0 for XYZ mode)'
            else:
                mode_str = f'Target: [X={node.x:.3f}, Y={node.y:.3f}, Z={node.z:.3f}]'
            servos = node.servo_display()
            # Move cursor up 2 lines, clear both, print fresh
            sys.stdout.write(f'\033[2A\033[K{mode_str}\n\033[K{servos}\n')
            sys.stdout.flush()

    except Exception as e:
        print(e)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
        print('\n')
        node.destroy_node()
        rclpy.shutdown()
