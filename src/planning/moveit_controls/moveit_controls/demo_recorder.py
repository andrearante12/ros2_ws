#!/usr/bin/env python3
"""
Demo recorder for imitation learning.

Runs alongside mujoco_mirror.launch.py while a human teleoperates.
Records observation-action pairs at 10 Hz and saves episodes as .pt files.

Observation (13D):
  - 7 joint positions (joint_1-4, end_effector_joint, end_effector_top/bottom)
  - 3D end-effector position (from TF: end_effector_link in base_link frame)
  - 3D object position (from /object/position)

Action (4D):
  - delta XYZ (ee_pos[t+1] - ee_pos[t])
  - gripper state (0.0 = open, 1.0 = closed)

Controls:
  Enter   — start/stop recording
  R       — reset sim (randomize cube position)
  Ctrl+C  — quit

Usage:
  Terminal 1: ros2 launch arm_bringup mujoco_mirror.launch.py
  Terminal 2: ros2 run moveit_controls demo_recorder
"""

import os
import sys
import termios
import tty
import select
import threading

import json
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from geometry_msgs.msg import Point
from std_msgs.msg import Empty, String
import tf2_ros


JOINT_NAMES = [
    'joint_1', 'joint_2', 'joint_3', 'joint_4',
    'end_effector_joint', 'end_effector_top_joint', 'end_effector_bottom_joint',
]

EPISODES_DIR = os.path.expanduser('~/demonstrations/episodes')


class DemoRecorder(Node):
    def __init__(self):
        super().__init__('demo_recorder')

        # TF for end-effector position
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        # Subscribers
        self.create_subscription(JointState, '/joint_states', self._joint_cb, 10)
        self.create_subscription(Point, '/object/position', self._object_cb, 10)
        self.create_subscription(String, '/gripper/command', self._gripper_cb, 10)

        # Publisher for sim reset
        self.reset_pub = self.create_publisher(Empty, '/sim/reset', 10)

        # State
        self.joint_positions = {}
        self.object_pos = None
        self.gripper_closed = 0.0  # 0.0 = open, 1.0 = closed
        self.ee_pos = None

        # Recording state
        self.recording = False
        self.episode_obs = []
        self.episode_actions = []
        self.prev_ee_pos = None

        # 10 Hz recording timer
        self.record_timer = self.create_timer(0.1, self._record_tick)

        os.makedirs(EPISODES_DIR, exist_ok=True)

    def _joint_cb(self, msg: JointState):
        self.joint_positions = dict(zip(msg.name, msg.position))

    def _object_cb(self, msg: Point):
        self.object_pos = (msg.x, msg.y, msg.z)

    def _gripper_cb(self, msg: String):
        cmd = msg.data.strip().lower()
        if cmd == 'close':
            self.gripper_closed = 1.0
        elif cmd == 'open':
            self.gripper_closed = 0.0

    def _get_ee_pos(self):
        try:
            t = self.tf_buffer.lookup_transform('base_link', 'end_effector_link', rclpy.time.Time())
            p = t.transform.translation
            return (p.x, p.y, p.z)
        except Exception:
            return None

    def _record_tick(self):
        if not self.recording:
            return

        # Check all data is available
        joints = [self.joint_positions.get(j) for j in JOINT_NAMES]
        if any(v is None for v in joints):
            return
        if self.object_pos is None:
            return

        ee = self._get_ee_pos()
        if ee is None:
            return
        self.ee_pos = ee

        # Build observation: 7 joints + 3 EE + 3 object = 13
        obs = list(joints) + list(ee) + list(self.object_pos)

        # Build action: delta EE + gripper
        if self.prev_ee_pos is not None:
            dx = ee[0] - self.prev_ee_pos[0]
            dy = ee[1] - self.prev_ee_pos[1]
            dz = ee[2] - self.prev_ee_pos[2]
            action = [dx, dy, dz, self.gripper_closed]
            self.episode_obs.append(obs)
            self.episode_actions.append(action)

        self.prev_ee_pos = ee

    def start_recording(self):
        self.recording = True
        self.episode_obs = []
        self.episode_actions = []
        self.prev_ee_pos = None

    def stop_recording(self):
        self.recording = False
        return len(self.episode_obs)

    def save_episode(self, success: bool):
        if not self.episode_obs:
            return None

        # Find next episode number
        existing = [f for f in os.listdir(EPISODES_DIR) if f.startswith('episode_') and f.endswith('.json')]
        nums = []
        for f in existing:
            try:
                nums.append(int(f.replace('episode_', '').replace('.json', '')))
            except ValueError:
                pass
        next_num = max(nums) + 1 if nums else 0

        filename = f'episode_{next_num:04d}.json'
        filepath = os.path.join(EPISODES_DIR, filename)

        data = {
            'observations': self.episode_obs,
            'actions': self.episode_actions,
            'success': success,
        }
        with open(filepath, 'w') as f:
            json.dump(data, f)
        return filepath

    def reset_sim(self):
        self.reset_pub.publish(Empty())


def main(args=None):
    rclpy.init(args=args)
    node = DemoRecorder()

    # Spin ROS in a background thread
    spin_thread = threading.Thread(target=rclpy.spin, args=(node,), daemon=True)
    spin_thread.start()

    settings = termios.tcgetattr(sys.stdin)

    print('\n=== Demo Recorder ===')
    print('  Enter — start/stop recording')
    print('  R     — reset sim (randomize cube)')
    print('  Ctrl+C — quit')
    print('=====================\n')
    print('Waiting for data...')

    # Wait for initial data
    import time
    while node.object_pos is None or not node.joint_positions:
        time.sleep(0.1)
    print('Data received. Ready to record.\n')

    try:
        while True:
            # Non-blocking key read
            tty.setraw(sys.stdin.fileno())
            try:
                if select.select([sys.stdin], [], [], 0.1)[0]:
                    key = sys.stdin.read(1)
                else:
                    key = None
            finally:
                termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)

            if key is None:
                continue

            if key == '\r' or key == '\n':  # Enter
                if not node.recording:
                    node.start_recording()
                    print('[RECORDING] Teleoperate now... press Enter to stop')
                else:
                    count = node.stop_recording()
                    print(f'[STOPPED] Recorded {count} timesteps')
                    if count > 0:
                        # Ask success/failure
                        answer = input('  Success? (y/n): ').strip().lower()
                        success = answer == 'y'
                        path = node.save_episode(success)
                        label = 'SUCCESS' if success else 'FAIL'
                        print(f'  Saved [{label}]: {path}')
                        print(f'  Obs shape: [{count}, 13], Action shape: [{count}, 4]')
                    else:
                        print('  No data recorded, skipping save')
                    print()

            elif key in ('r', 'R'):
                node.reset_sim()
                print('[RESET] Cube randomized')

            elif key == '\x03':  # Ctrl+C
                break

    except KeyboardInterrupt:
        pass
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
        print('\nShutting down demo recorder')
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
