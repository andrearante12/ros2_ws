#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge
import cv2
import json
import numpy as np
import os
from ament_index_python.packages import get_package_share_directory

class YOLODetector(Node):
    def __init__(self):
        super().__init__('yolo_detector')
        
        # Declare parameters
        self.declare_parameter('model_path', '')
        self.declare_parameter('confidence_threshold', 0.4)
        self.declare_parameter('nms_threshold', 0.4)
        self.declare_parameter('image_topic', '/camera/camera/color/image_raw')
        self.declare_parameter('depth_topic', '/camera/camera/aligned_depth_to_color/image_raw')
        self.declare_parameter('publish_visualization', True)
        self.declare_parameter('frame_skip', 0)
        
        # Get parameters
        model_path = self.get_parameter('model_path').value
        if not model_path or model_path == '':
            # Default to chess model
            pkg_share = get_package_share_directory('yolov5_detection')
            model_path = os.path.join(pkg_share, 'models', 'chess_yolov11.onnx')
        
        self.conf_threshold = self.get_parameter('confidence_threshold').value
        self.nms_threshold = self.get_parameter('nms_threshold').value
        image_topic = self.get_parameter('image_topic').value
        depth_topic = self.get_parameter('depth_topic').value
        self.publish_viz = self.get_parameter('publish_visualization').value
        self.frame_skip = self.get_parameter('frame_skip').value
        
        # Hardcoded chess class names
        self.class_names = ['chess_king', 'chess_queen']
        self.get_logger().info(f'Using chess class names: {self.class_names}')
        
        # Load YOLO model using OpenCV DNN
        self.get_logger().info(f'Loading YOLO model from: {model_path}')
        if not os.path.exists(model_path):
            self.get_logger().error(f'Model file not found: {model_path}')
            raise FileNotFoundError(f'Model file not found: {model_path}')
            
        self.net = cv2.dnn.readNetFromONNX(model_path)
        self.net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
        self.net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)
        
        # Model input size
        self.input_size = (640, 640)
        
        # CV Bridge
        self.bridge = CvBridge()
        
        # Frame skipping
        self.frame_count = 0
        
        # Subscribers
        self.image_sub = self.create_subscription(
            Image,
            image_topic,
            self.image_callback,
            10
        )
        
        self.depth_sub = self.create_subscription(
            Image,
            depth_topic,
            self.depth_callback,
            10
        )
        
        # Publishers
        self.detection_pub = self.create_publisher(
            String,
            '/yolo/detections',
            10
        )
        
        if self.publish_viz:
            self.viz_pub = self.create_publisher(
                Image,
                '/yolo/visualization',
                10
            )
        
        self.depth_image = None
        self.get_logger().info('=== YOLO Chess Detector Initialized ===')
        self.get_logger().info(f'Confidence threshold: {self.conf_threshold}')
        self.get_logger().info(f'Number of classes: {len(self.class_names)}')
        self.get_logger().info(f'Classes: {self.class_names}')
    
    def depth_callback(self, msg):
        """Store latest depth image"""
        try:
            self.depth_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='passthrough')
        except Exception as e:
            self.get_logger().error(f'Error converting depth image: {e}')
    
    def preprocess(self, image):
        """Preprocess image for YOLO"""
        blob = cv2.dnn.blobFromImage(image, 1/255.0, self.input_size, swapRB=True, crop=False)
        return blob
    
    def postprocess(self, outputs, image_shape):
        """Post-process YOLOv11 ONNX outputs (transposed format)"""
        detections = []
        
        output = outputs[0]
        
        # YOLOv11 outputs in transposed format: (1, 7, 8400) instead of (1, 8400, 7)
        # Need to transpose to (8400, 7)
        if len(output.shape) == 3:
            # Remove batch dimension and transpose
            # From (1, 7, 8400) to (8400, 7)
            output = output[0].T
        elif len(output.shape) == 2 and output.shape[0] < output.shape[1]:
            # Already 2D but needs transpose: (7, 8400) to (8400, 7)
            output = output.T
        
        rows = output.shape[0]
        
        for i in range(rows):
            row = output[i]
            
            # YOLOv11 format: [x_center, y_center, width, height, conf_class1, conf_class2]
            # No separate objectness score - class confidences include objectness
            
            # Get class confidences (last 2 values for 2 classes)
            class_confidences = row[4:6]  # Only first 2 classes
            
            max_conf = np.max(class_confidences)
            
            if max_conf > self.conf_threshold:
                class_id = np.argmax(class_confidences)
                
                # Get box coordinates (normalized 0-1, relative to 640x640)
                cx = row[0]
                cy = row[1]
                w = row[2]
                h = row[3]
                
                # Convert from normalized coordinates to pixel coordinates
                # cx, cy, w, h are in 0-640 range (model input size)
                x1 = int((cx - w/2) * image_shape[1] / 640)
                y1 = int((cy - h/2) * image_shape[0] / 640)
                x2 = int((cx + w/2) * image_shape[1] / 640)
                y2 = int((cy + h/2) * image_shape[0] / 640)
                
                # Clamp to image boundaries
                x1 = max(0, min(x1, image_shape[1]))
                y1 = max(0, min(y1, image_shape[0]))
                x2 = max(0, min(x2, image_shape[1]))
                y2 = max(0, min(y2, image_shape[0]))
                
                detections.append({
                    'class_id': int(class_id),
                    'confidence': float(max_conf),
                    'box': [x1, y1, x2, y2]
                })
        
        # Apply Non-Maximum Suppression
        if len(detections) > 0:
            boxes = [d['box'] for d in detections]
            confidences = [d['confidence'] for d in detections]
            indices = cv2.dnn.NMSBoxes(boxes, confidences, self.conf_threshold, self.nms_threshold)
            
            if len(indices) > 0:
                indices = indices.flatten() if hasattr(indices, 'flatten') else indices
                return [detections[i] for i in indices]
        
        return []
    
    def image_callback(self, msg):
        """Process RGB image and run detection"""
        # Frame skipping
        if self.frame_skip > 0:
            self.frame_count += 1
            if self.frame_count % (self.frame_skip + 1) != 0:
                return
        
        try:
            # Convert ROS Image to OpenCV
            cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
            orig_shape = cv_image.shape
            
            # Preprocess
            blob = self.preprocess(cv_image)
            
            # Run inference
            self.net.setInput(blob)
            outputs = self.net.forward(self.net.getUnconnectedOutLayersNames())
            
            # Post-process
            detections = self.postprocess(outputs, orig_shape)
            
            detection_list = []
            
            for det in detections:
                x1, y1, x2, y2 = det['box']
                conf = det['confidence']
                cls_id = det['class_id']
                
                # Make sure class_id is within range
                if cls_id < len(self.class_names):
                    cls_name = self.class_names[cls_id]
                else:
                    cls_name = f"class_{cls_id}"
                    self.get_logger().warn(f'Class ID {cls_id} out of range (have {len(self.class_names)} classes)')
                
                # Calculate center
                center_x = int((x1 + x2) / 2)
                center_y = int((y1 + y2) / 2)
                
                # Get depth if available
                depth = None
                if self.depth_image is not None:
                    if 0 <= center_y < self.depth_image.shape[0] and 0 <= center_x < self.depth_image.shape[1]:
                        depth = float(self.depth_image[center_y, center_x] / 1000.0)
                
                # Create detection dict
                det_dict = {
                    'class': cls_name,
                    'class_id': cls_id,
                    'confidence': conf,
                    'bbox': {
                        'x1': x1, 'y1': y1,
                        'x2': x2, 'y2': y2,
                        'center_x': center_x,
                        'center_y': center_y
                    },
                    'depth_m': depth
                }
                detection_list.append(det_dict)
                
                # Draw on visualization
                if self.publish_viz:
                    # Draw bounding box
                    cv2.rectangle(cv_image, (x1, y1), (x2, y2), (0, 255, 0), 2)
                    
                    # Create label
                    label = f'{cls_name} {conf:.2f}'
                    if depth is not None and depth > 0:
                        label += f' {depth:.2f}m'
                    
                    # Draw label with background
                    font = cv2.FONT_HERSHEY_SIMPLEX
                    font_scale = 0.6
                    thickness = 2
                    
                    (label_width, label_height), baseline = cv2.getTextSize(label, font, font_scale, thickness)
                    
                    # Draw background rectangle
                    cv2.rectangle(cv_image, 
                                 (x1, y1 - label_height - 10), 
                                 (x1 + label_width, y1),
                                 (0, 255, 0), -1)
                    
                    # Draw text
                    cv2.putText(cv_image, label, (x1, y1 - 5), 
                               font, font_scale, (0, 0, 0), thickness)
            
            # Publish detections
            detection_msg = String()
            detection_msg.data = json.dumps({
                'timestamp': msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9,
                'detections': detection_list,
                'count': len(detection_list)
            })
            self.detection_pub.publish(detection_msg)
            
            # Publish visualization
            if self.publish_viz:
                viz_msg = self.bridge.cv2_to_imgmsg(cv_image, encoding='bgr8')
                viz_msg.header = msg.header
                self.viz_pub.publish(viz_msg)
                
            if len(detection_list) > 0:
                self.get_logger().info(f'Detected {len(detection_list)} objects: {[d["class"] for d in detection_list]}')
                
        except Exception as e:
            self.get_logger().error(f'Error in detection: {e}')
            import traceback
            self.get_logger().error(traceback.format_exc())

def main(args=None):
    rclpy.init(args=args)
    node = YOLODetector()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
