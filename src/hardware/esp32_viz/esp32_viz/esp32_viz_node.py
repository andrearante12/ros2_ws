#!/usr/bin/env python3
"""
ESP32 Wearable Controller — RViz Visualization

Subscribes to /odom (OTOS position) and /imu/data (orientation),
mirrors the esp32_controller coordinate mapping, and publishes
RViz markers showing:

  1. Green sphere  — target end-effector position in workspace
  2. Arrow         — wrist orientation from IMU roll
  3. Wireframe box — workspace bounding volume

Usage:
  ros2 run esp32_viz esp32_viz_node
  ros2 run esp32_viz esp32_viz_node --ros-args -p x_sensitivity:=3.0
"""

import math

import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from visualization_msgs.msg import Marker, MarkerArray
from geometry_msgs.msg import Point, Quaternion
from builtin_interfaces.msg import Duration


# Workspace bounds (must match esp32_controller.cpp)
TARGET_X_MIN = -0.08
TARGET_X_MAX = 0.09
TARGET_Y_MIN = 0.05
TARGET_Y_MAX = 0.20


def map_range(value, in_min, in_max, out_min, out_max):
    value = max(in_min, min(in_max, value))
    return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min)


def quaternion_from_euler(roll, pitch, yaw):
    """Create a quaternion from roll/pitch/yaw Euler angles (ZYX convention)."""
    cr, sr = math.cos(roll / 2.0), math.sin(roll / 2.0)
    cp, sp = math.cos(pitch / 2.0), math.sin(pitch / 2.0)
    cy, sy = math.cos(yaw / 2.0), math.sin(yaw / 2.0)
    return Quaternion(
        x=sr * cp * cy - cr * sp * sy,
        y=cr * sp * cy + sr * cp * sy,
        z=cr * cp * sy - sr * sp * cy,
        w=cr * cp * cy + sr * sp * sy,
    )


class Esp32VizNode(Node):

    def __init__(self):
        super().__init__("esp32_viz")

        # Parameters (mirror esp32_controller defaults)
        self.declare_parameter("x_sensitivity", 1.5)
        self.declare_parameter("y_sensitivity", 2.0)
        self.declare_parameter("default_z_position", 0.19)
        self.declare_parameter("default_y_position", -0.03)
        self.declare_parameter("lock_x_axis", False)
        self.declare_parameter("lock_y_axis", False)
        self.declare_parameter("lock_wrist", False)
        self.declare_parameter("wrist_sensitivity", 1.0)
        self.declare_parameter("default_wrist_angle", 90)

        self.x_sensitivity = self.get_parameter("x_sensitivity").value
        self.y_sensitivity = self.get_parameter("y_sensitivity").value
        self.default_z = self.get_parameter("default_z_position").value
        self.default_y = self.get_parameter("default_y_position").value
        self.lock_x = self.get_parameter("lock_x_axis").value
        self.lock_y = self.get_parameter("lock_y_axis").value
        self.lock_wrist = self.get_parameter("lock_wrist").value
        self.wrist_sensitivity = self.get_parameter("wrist_sensitivity").value
        self.default_wrist_angle = self.get_parameter("default_wrist_angle").value

        # OTOS calibration state (mirrors esp32_controller dynamic range tracking)
        self.first_odom = True
        self.odom_x_min = 0.0
        self.odom_x_max = 0.0
        self.odom_y_min = 0.0
        self.odom_y_max = 0.0

        # Current target state
        self.target_x = (TARGET_X_MIN + TARGET_X_MAX) / 2.0
        self.target_y = (TARGET_Y_MIN + TARGET_Y_MAX) / 2.0
        self.target_z = self.default_z
        self.imu_roll = 0.0
        self.imu_pitch = 0.0

        # Publisher
        self.marker_pub = self.create_publisher(MarkerArray, "/esp32_viz/markers", 10)

        # Subscribers
        self.create_subscription(Odometry, "/odom", self.odom_callback, 10)
        self.create_subscription(Imu, "/imu/data", self.imu_callback, 10)

        # Publish bounding box at 1Hz (static geometry)
        self.create_timer(1.0, self.publish_bounding_box)

        self.get_logger().info("ESP32 visualization ready — add MarkerArray '/esp32_viz/markers' in RViz")

    def imu_callback(self, msg):
        # Extract roll and pitch from quaternion
        qx = msg.orientation.x
        qy = msg.orientation.y
        qz = msg.orientation.z
        qw = msg.orientation.w

        # Roll (X-axis rotation)
        sinr_cosp = 2.0 * (qw * qx + qy * qz)
        cosr_cosp = 1.0 - 2.0 * (qx * qx + qy * qy)
        self.imu_roll = math.atan2(sinr_cosp, cosr_cosp)

        # Pitch (Y-axis rotation)
        sinp = 2.0 * (qw * qy - qz * qx)
        if abs(sinp) >= 1:
            self.imu_pitch = math.copysign(math.pi / 2, sinp)
        else:
            self.imu_pitch = math.asin(sinp)

    def odom_callback(self, msg):
        odom_x = msg.pose.pose.position.x
        odom_y = msg.pose.pose.position.y

        # Dynamic range calibration (mirrors esp32_controller)
        if self.first_odom:
            self.odom_x_min = self.odom_x_max = odom_x
            self.odom_y_min = self.odom_y_max = odom_y
            self.first_odom = False
        else:
            self.odom_x_min = min(self.odom_x_min, odom_x)
            self.odom_x_max = max(self.odom_x_max, odom_x)
            self.odom_y_min = min(self.odom_y_min, odom_y)
            self.odom_y_max = max(self.odom_y_max, odom_y)

        odom_x_center = (self.odom_x_min + self.odom_x_max) / 2.0
        odom_y_center = (self.odom_y_min + self.odom_y_max) / 2.0
        odom_x_range = (self.odom_x_max - self.odom_x_min) / self.x_sensitivity
        odom_y_range = (self.odom_y_max - self.odom_y_min) / self.y_sensitivity

        # Avoid division by zero when range is still zero
        if odom_x_range > 1e-6:
            scaled_x = map_range(
                odom_x,
                odom_x_center - odom_x_range, odom_x_center + odom_x_range,
                TARGET_X_MIN, TARGET_X_MAX,
            )
        else:
            scaled_x = (TARGET_X_MIN + TARGET_X_MAX) / 2.0

        if odom_y_range > 1e-6:
            scaled_y = map_range(
                odom_y,
                odom_y_center - odom_y_range, odom_y_center + odom_y_range,
                TARGET_Y_MIN, TARGET_Y_MAX,
            )
        else:
            scaled_y = (TARGET_Y_MIN + TARGET_Y_MAX) / 2.0

        self.target_x = scaled_x if not self.lock_x else self.target_x
        self.target_y = self.default_y if self.lock_y else scaled_y
        self.target_z = self.default_z

        self.publish_target_markers()

    def publish_target_markers(self):
        markers = MarkerArray()
        now = self.get_clock().now().to_msg()
        lifetime = Duration(sec=0, nanosec=200_000_000)  # 200ms

        # 1. Target sphere
        sphere = Marker()
        sphere.header.stamp = now
        sphere.header.frame_id = "base_link"
        sphere.ns = "esp32_target"
        sphere.id = 0
        sphere.type = Marker.SPHERE
        sphere.action = Marker.ADD
        sphere.pose.position.x = self.target_x
        sphere.pose.position.y = self.target_y
        sphere.pose.position.z = self.target_z
        sphere.pose.orientation.w = 1.0
        sphere.scale.x = 0.06
        sphere.scale.y = 0.06
        sphere.scale.z = 0.06
        sphere.color.r = 0.2
        sphere.color.g = 1.0
        sphere.color.b = 0.2
        sphere.color.a = 0.9
        sphere.lifetime = lifetime
        markers.markers.append(sphere)

        # 2. Orientation arrow (roll + pitch from IMU)
        arrow = Marker()
        arrow.header.stamp = now
        arrow.header.frame_id = "base_link"
        arrow.ns = "esp32_target"
        arrow.id = 1
        arrow.type = Marker.ARROW
        arrow.action = Marker.ADD
        arrow.pose.position.x = self.target_x
        arrow.pose.position.y = self.target_y
        arrow.pose.position.z = self.target_z
        arrow.pose.orientation = quaternion_from_euler(self.imu_roll, self.imu_pitch, 0.0)
        arrow.scale.x = 0.12  # shaft length
        arrow.scale.y = 0.012  # shaft diameter
        arrow.scale.z = 0.018  # head diameter
        arrow.color.r = 1.0
        arrow.color.g = 0.6
        arrow.color.b = 0.0
        arrow.color.a = 0.9
        arrow.lifetime = lifetime
        markers.markers.append(arrow)

        self.marker_pub.publish(markers)

    def publish_bounding_box(self):
        box = Marker()
        box.header.stamp = self.get_clock().now().to_msg()
        box.header.frame_id = "base_link"
        box.ns = "esp32_workspace"
        box.id = 0
        box.type = Marker.LINE_LIST
        box.action = Marker.ADD
        box.scale.x = 0.005  # line width
        box.color.r = 0.5
        box.color.g = 0.5
        box.color.b = 1.0
        box.color.a = 0.6
        box.pose.orientation.w = 1.0
        box.lifetime = Duration(sec=2, nanosec=0)

        x0, x1 = TARGET_X_MIN, TARGET_X_MAX
        y0, y1 = TARGET_Y_MIN, TARGET_Y_MAX
        z = self.default_z
        # Z extent for visible 3D wireframe
        z0 = z - 0.10
        z1 = z + 0.10

        # 12 edges of a rectangular prism
        corners = [
            (x0, y0, z0), (x1, y0, z0), (x1, y1, z0), (x0, y1, z0),
            (x0, y0, z1), (x1, y0, z1), (x1, y1, z1), (x0, y1, z1),
        ]
        edges = [
            (0, 1), (1, 2), (2, 3), (3, 0),  # bottom
            (4, 5), (5, 6), (6, 7), (7, 4),  # top
            (0, 4), (1, 5), (2, 6), (3, 7),  # verticals
        ]
        for a, b in edges:
            box.points.append(Point(x=corners[a][0], y=corners[a][1], z=corners[a][2]))
            box.points.append(Point(x=corners[b][0], y=corners[b][1], z=corners[b][2]))

        markers = MarkerArray()
        markers.markers.append(box)
        self.marker_pub.publish(markers)


def main(args=None):
    rclpy.init(args=args)
    node = Esp32VizNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
