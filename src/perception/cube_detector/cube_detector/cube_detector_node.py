#!/usr/bin/env python3
"""
Detect a black cube on a table using HSV color segmentation + RealSense depth.

Publishes the cube's 3D position (in base_link frame) to /object/position,
matching the interface expected by demo_recorder and policy_executor.
"""

import numpy as np
import cv2

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image, CameraInfo
from geometry_msgs.msg import Point, PointStamped
from cv_bridge import CvBridge
import tf2_ros
import tf2_geometry_msgs  # noqa: F401 — registers PointStamped transform


class CubeDetector(Node):
    def __init__(self):
        super().__init__('cube_detector')

        self.declare_parameter('v_max', 50)
        self.declare_parameter('s_max', 80)
        self.declare_parameter('min_area', 100)
        self.declare_parameter('max_area', 5000)
        self.declare_parameter('stale_timeout', 2.0)
        self.declare_parameter('publish_debug', False)

        self.v_max = self.get_parameter('v_max').value
        self.s_max = self.get_parameter('s_max').value
        self.min_area = self.get_parameter('min_area').value
        self.max_area = self.get_parameter('max_area').value
        self.stale_timeout = self.get_parameter('stale_timeout').value
        self.publish_debug = self.get_parameter('publish_debug').value

        self.bridge = CvBridge()
        self.latest_rgb = None
        self.latest_depth = None
        self.intrinsics = None  # (fx, fy, cx, cy)
        self.camera_frame = None
        self.last_position = None
        self.last_seen_time = None

        # TF
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        # Subscribers
        self.create_subscription(
            Image, '/camera/camera/color/image_raw', self._rgb_cb, 10)
        self.create_subscription(
            Image, '/camera/camera/aligned_depth_to_color/image_raw', self._depth_cb, 10)
        self.create_subscription(
            CameraInfo, '/camera/camera/color/camera_info', self._info_cb, 10)

        # Publishers
        self.pos_pub = self.create_publisher(Point, '/object/position', 10)
        if self.publish_debug:
            self.debug_pub = self.create_publisher(Image, '/cube_detector/debug', 10)

        # 10Hz detection timer
        self.create_timer(0.1, self._detect)

        self.get_logger().info(
            f'Cube detector started (V<{self.v_max}, S<{self.s_max}, '
            f'area {self.min_area}-{self.max_area})')

    def _rgb_cb(self, msg: Image):
        self.latest_rgb = msg

    def _depth_cb(self, msg: Image):
        self.latest_depth = msg

    def _info_cb(self, msg: CameraInfo):
        if self.intrinsics is None:
            self.intrinsics = (msg.k[0], msg.k[4], msg.k[2], msg.k[5])
            self.camera_frame = msg.header.frame_id
            self.get_logger().info(
                f'Camera intrinsics: fx={msg.k[0]:.1f} fy={msg.k[4]:.1f} '
                f'cx={msg.k[2]:.1f} cy={msg.k[5]:.1f} frame={self.camera_frame}')

    def _detect(self):
        if self.latest_rgb is None or self.latest_depth is None or self.intrinsics is None:
            return

        rgb = self.bridge.imgmsg_to_cv2(self.latest_rgb, 'bgr8')
        depth = self.bridge.imgmsg_to_cv2(self.latest_depth, 'passthrough')

        # HSV threshold for black cube
        hsv = cv2.cvtColor(rgb, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, np.array([0, 0, 0]), np.array([180, self.s_max, self.v_max]))

        # Clean up noise
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
        mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

        # Find contours
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # Filter by area, pick largest
        best = None
        best_area = 0
        for c in contours:
            area = cv2.contourArea(c)
            if self.min_area <= area <= self.max_area and area > best_area:
                best = c
                best_area = area

        now = self.get_clock().now()

        if best is not None:
            M = cv2.moments(best)
            if M['m00'] > 0:
                u = int(M['m10'] / M['m00'])
                v = int(M['m01'] / M['m00'])

                # Sample depth: median of 5x5 patch
                h, w = depth.shape
                y0, y1 = max(0, v - 2), min(h, v + 3)
                x0, x1 = max(0, u - 2), min(w, u + 3)
                patch = depth[y0:y1, x0:x1].astype(float)
                valid = patch[patch > 0]

                if len(valid) > 0:
                    z_m = float(np.median(valid)) / 1000.0  # mm → meters

                    fx, fy, cx, cy = self.intrinsics
                    x_cam = (u - cx) * z_m / fx
                    y_cam = (v - cy) * z_m / fy

                    # Transform to base_link
                    pt = PointStamped()
                    pt.header.stamp = now.to_msg()
                    pt.header.frame_id = self.camera_frame
                    pt.point.x = x_cam
                    pt.point.y = y_cam
                    pt.point.z = z_m

                    try:
                        pt_base = self.tf_buffer.transform(pt, 'base_link', timeout=rclpy.duration.Duration(seconds=0.1))
                        msg = Point(x=pt_base.point.x, y=pt_base.point.y, z=pt_base.point.z)
                        self.pos_pub.publish(msg)
                        self.last_position = msg
                        self.last_seen_time = now
                    except Exception as e:
                        self.get_logger().warning(f'TF failed: {e}', throttle_duration_sec=5.0)

                if self.publish_debug:
                    cv2.drawContours(rgb, [best], -1, (0, 255, 0), 2)
                    cv2.circle(rgb, (u, v), 5, (0, 0, 255), -1)

        elif self.last_position is not None and self.last_seen_time is not None:
            elapsed = (now - self.last_seen_time).nanoseconds / 1e9
            if elapsed < self.stale_timeout:
                self.pos_pub.publish(self.last_position)
            else:
                self.get_logger().warning('Cube not detected — stale timeout', throttle_duration_sec=5.0)

        if self.publish_debug:
            debug_msg = self.bridge.cv2_to_imgmsg(rgb, 'bgr8')
            debug_msg.header.stamp = now.to_msg()
            self.debug_pub.publish(debug_msg)


def main(args=None):
    rclpy.init(args=args)
    node = CubeDetector()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
