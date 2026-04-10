import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Float64MultiArray


OPEN_CMD  = [-0.35,  0.35]
CLOSE_CMD = [0.09, -0.09]   # fingers close inward


class GripperControl(Node):
    def __init__(self):
        super().__init__('gripper_control')

        self.cmd_pub = self.create_publisher(
            Float64MultiArray, '/finger_controller/commands', 10)

        self.create_subscription(
            String, '/gripper/command', self._command_cb, 10)

        self.get_logger().info("Gripper control ready — publish 'open' or 'close' to /gripper/command")

    def _command_cb(self, msg: String):
        cmd = msg.data.strip().lower()
        if cmd == 'open':
            self.get_logger().info('Gripper opening')
            self._publish(OPEN_CMD)
        elif cmd == 'close':
            self.get_logger().info('Gripper closing')
            self._publish(CLOSE_CMD)
        else:
            self.get_logger().warn(f"Unknown command: '{cmd}' (use 'open' or 'close')")

    def _publish(self, values):
        msg = Float64MultiArray()
        msg.data = values
        self.cmd_pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = GripperControl()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
