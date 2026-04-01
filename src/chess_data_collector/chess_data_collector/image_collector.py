#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import os

class TimedCollector(Node):
    def __init__(self):
        super().__init__('timed_collector')
        
        self.declare_parameter('save_dir', '~/chess_dataset/images')
        self.declare_parameter('interval', 3.0)  # Save every 3 seconds
        self.declare_parameter('image_topic', '/camera/camera/color/image_raw')
        
        save_dir = os.path.expanduser(self.get_parameter('save_dir').value)
        interval = self.get_parameter('interval').value
        image_topic = self.get_parameter('image_topic').value
        
        os.makedirs(save_dir, exist_ok=True)
        
        self.save_dir = save_dir
        self.bridge = CvBridge()
        self.image_count = len([f for f in os.listdir(save_dir) if f.endswith('.jpg')])
        self.current_image = None
        
        self.subscription = self.create_subscription(
            Image,
            image_topic,
            self.image_callback,
            10)
        
        self.timer = self.create_timer(interval, self.save_image)
        
        self.get_logger().info('=== Auto Chess Collector ===')
        self.get_logger().info(f'Saving every {interval}s to: {save_dir}')
        self.get_logger().info(f'Starting at image: {self.image_count}')
        self.get_logger().info('Move chess pieces around! Press Ctrl+C to stop.')
        
    def image_callback(self, msg):
        self.current_image = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
    
    def save_image(self):
        if self.current_image is not None:
            filename = f'{self.save_dir}/chess_{self.image_count:04d}.jpg'
            cv2.imwrite(filename, self.current_image)
            self.get_logger().info(f'✓ Saved: chess_{self.image_count:04d}.jpg')
            self.image_count += 1
        else:
            self.get_logger().warn('No image received yet!')

def main(args=None):
    rclpy.init(args=args)
    node = TimedCollector()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info(f'Stopped. Total images: {node.image_count}')
    finally:
        node.destroy_node()

if __name__ == '__main__':
    main()
