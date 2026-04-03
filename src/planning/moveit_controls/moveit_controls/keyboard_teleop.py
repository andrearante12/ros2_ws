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

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Point
from std_msgs.msg import String
from visualization_msgs.msg import Marker, MarkerArray
from builtin_interfaces.msg import Duration


# Workspace bounds (from CLAUDE.md)
WS_X_MIN, WS_X_MAX = -0.08, 0.09
WS_Y_MIN, WS_Y_MAX = -0.18, 0.18
WS_Z_MIN, WS_Z_MAX = 0.05, 0.25

HELP_TEXT = """
Keyboard Teleop — Arm Target Control
-------------------------------------
  W/S : Y forward / backward
  A/D : X left / right
  Q/E : Z up / down
  O   : Open gripper
  C   : Close gripper
  R   : Reset to center
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
        self.gripper_pub = self.create_publisher(String, '/gripper/command', 10)
        self.marker_pub = self.create_publisher(MarkerArray, '/esp32_viz/markers', 10)

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

    try:
        while rclpy.ok():
            key = get_key(settings)
            moved = True

            if key == 'w':
                node.y += node.step
            elif key == 's':
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
                msg = String(data='open')
                node.gripper_pub.publish(msg)
                print('\rGripper: OPEN                              ', end='', flush=True)
                moved = False
            elif key == 'c':
                msg = String(data='close')
                node.gripper_pub.publish(msg)
                print('\rGripper: CLOSE                             ', end='', flush=True)
                moved = False
            elif key == 'r':
                node.reset()
            elif key == '\x03':  # Ctrl+C
                break
            else:
                moved = False

            if moved:
                node.publish()
                print(f'\rTarget: [X={node.x:.3f}, Y={node.y:.3f}, Z={node.z:.3f}]', end='', flush=True)

    except Exception as e:
        print(e)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
        print('\n')
        node.destroy_node()
        rclpy.shutdown()
