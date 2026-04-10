#!/usr/bin/env python3
"""
Policy executor for trained behavioral cloning model.

Runs the learned cube-lift policy at 10 Hz, then executes a hardcoded
place sequence once the lift succeeds.

Drop-in replacement for keyboard_teleop — publishes to the same topics:
  /move_to (Point) and /gripper/command (String)

Usage:
  ros2 run moveit_controls policy_executor --ros-args -p model_path:=~/models/bc_cube_lift/best.pt
"""

import os

import torch
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from geometry_msgs.msg import Point
from std_msgs.msg import String, Empty, Bool
import tf2_ros

from moveit_controls.il.policy import load_policy


JOINT_NAMES = [
    'joint_1', 'joint_2', 'joint_3', 'joint_4',
    'end_effector_joint', 'end_effector_top_joint', 'end_effector_bottom_joint',
]

# Cube z threshold for successful lift (above table surface)
LIFT_THRESHOLD_Z = 0.02  # in MuJoCo world frame; cube starts at ~-0.037
LIFT_HOLD_STEPS = 10     # must stay above threshold for 1 second (10 steps at 10 Hz)
MAX_POLICY_STEPS = 200   # 20 seconds max

# Hardcoded place waypoints after successful lift
PLACE_WAYPOINTS = [
    {'move': [0.0, 0.10, 0.20]},    # move above drop zone
    {'move': [0.0, 0.10, 0.12]},    # lower
    {'gripper': 'open'},             # release
    {'move': [0.0, 0.10, 0.20]},    # retract up
]


class PolicyExecutor(Node):
    def __init__(self):
        super().__init__('policy_executor')

        self.declare_parameter('model_path',
                               os.path.expanduser('~/models/bc_cube_lift/best.pt'))

        model_path = self.get_parameter('model_path').get_parameter_value().string_value
        model_path = os.path.expanduser(model_path)

        self.get_logger().info(f'Loading policy from {model_path}')
        self.policy, self.stats = load_policy(model_path)
        self.obs_mean = self.stats['obs_mean']
        self.obs_std = self.stats['obs_std']
        self.act_mean = self.stats['act_mean']
        self.act_std = self.stats['act_std']
        self.get_logger().info('Policy loaded')

        # TF
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        # Subscribers
        self.create_subscription(JointState, '/joint_states', self._joint_cb, 10)
        self.create_subscription(Point, '/object/position', self._object_cb, 10)
        self.create_subscription(Bool, '/move_to/result', self._move_result_cb, 10)

        # Publishers
        self.move_pub = self.create_publisher(Point, '/move_to', 10)
        self.gripper_pub = self.create_publisher(String, '/gripper/command', 10)
        self.reset_pub = self.create_publisher(Empty, '/sim/reset', 10)

        # State
        self.joint_positions = {}
        self.object_pos = None
        self.ee_pos = None
        self.move_done = True

        # Episode state
        self.step_count = 0
        self.lift_count = 0
        self.phase = 'lift'  # 'lift', 'place', 'done'
        self.place_idx = 0
        self.waiting_for_move = False

        # 10 Hz policy timer
        self.timer = self.create_timer(0.1, self._policy_tick)

        self.get_logger().info('Policy executor ready — running lift policy at 10 Hz')

    def _joint_cb(self, msg: JointState):
        self.joint_positions = dict(zip(msg.name, msg.position))

    def _object_cb(self, msg: Point):
        self.object_pos = (msg.x, msg.y, msg.z)

    def _move_result_cb(self, msg: Bool):
        self.move_done = True

    def _get_ee_pos(self):
        try:
            t = self.tf_buffer.lookup_transform('base_link', 'end_effector_link', rclpy.time.Time())
            p = t.transform.translation
            return (p.x, p.y, p.z)
        except Exception:
            return None

    def _policy_tick(self):
        if self.phase == 'done':
            return

        if self.phase == 'place':
            self._place_tick()
            return

        # === Lift phase: run learned policy ===
        joints = [self.joint_positions.get(j) for j in JOINT_NAMES]
        if any(v is None for v in joints) or self.object_pos is None:
            return

        ee = self._get_ee_pos()
        if ee is None:
            return
        self.ee_pos = ee

        # Build observation
        obs = torch.tensor(
            list(joints) + list(ee) + list(self.object_pos),
            dtype=torch.float32
        )

        # Normalize, forward pass, denormalize
        obs_norm = (obs - self.obs_mean) / self.obs_std
        with torch.no_grad():
            act_norm = self.policy(obs_norm.unsqueeze(0)).squeeze(0)
        act = act_norm * self.act_std + self.act_mean

        dx, dy, dz = act[0].item(), act[1].item(), act[2].item()
        gripper_val = act[3].item()

        # Publish move target (absolute = current EE + delta)
        target = Point(
            x=ee[0] + dx,
            y=ee[1] + dy,
            z=ee[2] + dz,
        )
        self.move_pub.publish(target)

        # Publish gripper command based on threshold
        if gripper_val > 0.5:
            self.gripper_pub.publish(String(data='close'))
        else:
            self.gripper_pub.publish(String(data='open'))

        self.step_count += 1

        # Check lift success
        if self.object_pos[2] > LIFT_THRESHOLD_Z:
            self.lift_count += 1
        else:
            self.lift_count = 0

        if self.lift_count >= LIFT_HOLD_STEPS:
            self.get_logger().info(
                f'LIFT SUCCESS at step {self.step_count}! '
                f'Cube z={self.object_pos[2]:.3f} > {LIFT_THRESHOLD_Z}'
            )
            self.phase = 'place'
            self.place_idx = 0
            self.waiting_for_move = False
            return

        if self.step_count >= MAX_POLICY_STEPS:
            self.get_logger().warn(
                f'Lift failed after {MAX_POLICY_STEPS} steps. '
                f'Cube z={self.object_pos[2]:.3f}'
            )
            self.phase = 'done'

    def _place_tick(self):
        """Execute hardcoded place waypoints."""
        if self.waiting_for_move and not self.move_done:
            return

        if self.place_idx >= len(PLACE_WAYPOINTS):
            self.get_logger().info('Place sequence complete!')
            self.phase = 'done'
            return

        wp = PLACE_WAYPOINTS[self.place_idx]
        self.place_idx += 1

        if 'move' in wp:
            x, y, z = wp['move']
            self.get_logger().info(f'Place: moving to [{x}, {y}, {z}]')
            self.move_pub.publish(Point(x=float(x), y=float(y), z=float(z)))
            self.move_done = False
            self.waiting_for_move = True
        elif 'gripper' in wp:
            self.get_logger().info(f'Place: gripper {wp["gripper"]}')
            self.gripper_pub.publish(String(data=wp['gripper']))
            self.waiting_for_move = False


def main(args=None):
    rclpy.init(args=args)
    node = PolicyExecutor()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
