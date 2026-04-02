import sys
import time
import subprocess
import yaml
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from sensor_msgs.msg import JointState
from ament_index_python.packages import get_package_share_directory

SETTLE_VEL_THRESHOLD = 0.01   # rad/s — consider finger settled below this
SETTLE_DURATION      = 0.5    # seconds finger must be settled
GRIPPER_TIMEOUT      = 10.0   # max wait for gripper


class TrajectoryRunner(Node):
    def __init__(self):
        super().__init__('trajectory_runner')
        self.gripper_pub = self.create_publisher(String, '/gripper/command', 10)
        self.create_subscription(JointState, '/joint_states', self._joint_state_cb, 10)
        self._top_vel = 0.0
        self._bot_vel = 0.0
        # Wait for gripper_control subscriber to connect
        while self.gripper_pub.get_subscription_count() == 0:
            self.get_logger().info('Waiting for gripper_control subscriber...', throttle_duration_sec=2.0)
            rclpy.spin_once(self, timeout_sec=0.1)
        self.get_logger().info('Gripper subscriber connected')

    def _joint_state_cb(self, msg: JointState):
        for i, name in enumerate(msg.name):
            if name == 'end_effector_top_joint':
                self._top_vel = msg.velocity[i] if msg.velocity else 0.0
            elif name == 'end_effector_bottom_joint':
                self._bot_vel = msg.velocity[i] if msg.velocity else 0.0

    def run_gripper(self, cmd):
        self.get_logger().info(f'Gripper: {cmd}')
        msg = String()
        msg.data = cmd
        self.gripper_pub.publish(msg)

        # Let controller start moving before checking velocity
        for _ in range(20):
            rclpy.spin_once(self, timeout_sec=0.05)

        # Wait until fingers stop moving
        settled_since = None
        start = time.time()
        while time.time() - start < GRIPPER_TIMEOUT:
            rclpy.spin_once(self, timeout_sec=0.05)
            both_settled = (abs(self._top_vel) < SETTLE_VEL_THRESHOLD and
                           abs(self._bot_vel) < SETTLE_VEL_THRESHOLD)
            if both_settled:
                if settled_since is None:
                    settled_since = time.time()
                elif time.time() - settled_since >= SETTLE_DURATION:
                    self.get_logger().info(f'Gripper {cmd} complete')
                    return
            else:
                settled_since = None

        self.get_logger().warn(f'Gripper {cmd} timed out after {GRIPPER_TIMEOUT}s')

    def run_move(self, coords):
        x, y, z = coords
        self.get_logger().info(f'Moving to: [{x}, {y}, {z}]')
        result = subprocess.run(
            ['ros2', 'run', 'move_program', 'move_with_simulation',
             '--', str(x), str(y), str(z)],
            capture_output=False
        )
        if result.returncode != 0:
            self.get_logger().error(f'Move failed with return code {result.returncode}')

    def run_trajectory(self, trajectory_file):
        with open(trajectory_file, 'r') as f:
            traj = yaml.safe_load(f)

        steps = traj.get('steps', [])
        self.get_logger().info(f'Running trajectory with {len(steps)} steps from {trajectory_file}')

        for i, step in enumerate(steps):
            self.get_logger().info(f'--- Step {i+1}/{len(steps)} ---')

            if 'gripper' in step:
                self.run_gripper(step['gripper'])
            elif 'move' in step:
                self.run_move(step['move'])
            elif 'sleep' in step:
                self.get_logger().info(f'Sleeping {step["sleep"]}s')
                time.sleep(step['sleep'])

        self.get_logger().info('Trajectory complete!')


def main(args=None):
    rclpy.init(args=args)

    if len(sys.argv) < 2:
        pkg_dir = get_package_share_directory('moveit_controls')
        trajectory_file = f'{pkg_dir}/trajectories/pick_and_place.yaml'
    else:
        trajectory_file = sys.argv[1]

    node = TrajectoryRunner()
    node.run_trajectory(trajectory_file)
    node.destroy_node()
    rclpy.shutdown()
