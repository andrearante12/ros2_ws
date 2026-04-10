#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from std_msgs.msg import String
import math
import serial
import time


# (servo_name, direction, offset, clamp_range)
# final_angle = direction * mujoco_degrees + offset
JOINT_SERVO_MAP = {
    'joint_1':                ('servo0', +1,  90, None),
    'joint_2':                ('servo1', -1,  50, None),
    'joint_3':                ('servo2', -1,  25, None),
    'joint_4':                ('servo3', -1,  55, None),
    'end_effector_joint':     ('servo4', -1, 210, None),
    # servo5 (gripper) is controlled directly via /arm/servo_commands, not joint_states

}


class PosePrinter(Node):
    def __init__(self):
        super().__init__('pose_printer')

        # ===== Parameters =====
        self.declare_parameter('serial_port', '/dev/ttyUSB0')
        self.declare_parameter('baud_rate', 115200)
        # dry_run: skip opening serial, log commands instead (useful for testing)
        self.declare_parameter('dry_run', False)
        # mode: 'joint_states' - 1 Hz timer from /joint_states (original behavior)
        #       'direct'       - only relay commands from /arm/servo_commands
        #       'both'         - subscribe to both (direct commands take priority)
        self.declare_parameter('mode', 'joint_states')

        serial_port = self.get_parameter('serial_port').get_parameter_value().string_value
        baud_rate = self.get_parameter('baud_rate').get_parameter_value().integer_value
        self.dry_run = self.get_parameter('dry_run').get_parameter_value().bool_value
        self.mode = self.get_parameter('mode').get_parameter_value().string_value

        self.get_logger().info(
            f"pose_printer starting: mode={self.mode}, dry_run={self.dry_run}, "
            f"port={serial_port}, baud={baud_rate}"
        )

        # ===== Serial setup =====
        self.ser = None
        if not self.dry_run:
            try:
                self.ser = serial.Serial(serial_port, baud_rate, timeout=1)
                time.sleep(2)  # allow Arduino reset
                self.get_logger().info(f"Connected to {serial_port}")
            except serial.SerialException as e:
                self.get_logger().error(f"Serial error: {e}")
        else:
            self.get_logger().info("dry_run=True: serial port will not be opened")

        # ===== ROS setup =====
        self.latest_joint_state = None

        if self.mode in ('joint_states', 'both'):
            self.joint_sub = self.create_subscription(
                JointState,
                '/joint_states',
                self.joint_state_callback,
                10
            )
            self.timer = self.create_timer(0.05, self.timer_callback)

        if self.mode in ('direct', 'both'):
            self.cmd_sub = self.create_subscription(
                String,
                '/arm/servo_commands',
                self.servo_command_callback,
                10
            )

    def write_command(self, command: str):
        """Write a servo command string to serial."""
        if self.ser:
            self.ser.write(command.encode())
            self.ser.flush()

    def servo_command_callback(self, msg: String):
        """Relay raw command strings received on /arm/servo_commands to serial."""
        for line in msg.data.splitlines():
            line = line.strip()
            if line:
                self.write_command(line + '\n')

    def joint_state_callback(self, msg: JointState):
        self.latest_joint_state = msg

    def timer_callback(self):
        if self.latest_joint_state is None:
            return

        joint_positions = dict(
            zip(self.latest_joint_state.name, self.latest_joint_state.position)
        )

        # Build servo commands from the explicit map, sorted by servo number
        commands = []
        for joint_name, (servo_name, direction, offset, clamp_range) in JOINT_SERVO_MAP.items():
            if joint_name not in joint_positions:
                continue
            angle = round(direction * math.degrees(joint_positions[joint_name]) + offset)
            if clamp_range is not None:
                angle = max(clamp_range[0], min(clamp_range[1], angle))
            commands.append((servo_name, angle))

        commands.sort(key=lambda c: c[0])
        for servo_name, angle in commands:
            self.write_command(f"{servo_name}={angle}\n")

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
