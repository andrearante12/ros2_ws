#!/usr/bin/env python3
from flask import Flask, render_template, Response, jsonify
from flask_cors import CORS
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image, JointState
from cv_bridge import CvBridge
import cv2
import threading
import numpy as np

app = Flask(__name__)
CORS(app)

class ROSBridge(Node):
    def __init__(self):
        super().__init__('web_interface_bridge')
        self.bridge = CvBridge()
        
        # Latest frames
        self.color_frame = None
        self.depth_frame = None
        self.joint_states = None
        self.frame_lock = threading.Lock()
        
        # Subscribers
        self.color_sub = self.create_subscription(
            Image,
            '/camera/camera/color/image_raw',
            self.color_callback,
            10)
        
        self.depth_sub = self.create_subscription(
            Image,
            '/camera/camera/depth/image_rect_raw',
            self.depth_callback,
            10)
        
        self.joint_sub = self.create_subscription(
            JointState,
            '/joint_states',
            self.joint_callback,
            10)
        
        self.get_logger().info('ROS Bridge initialized')
    
    def color_callback(self, msg):
        try:
            frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
            # Resize for faster streaming (optional)
            frame = cv2.resize(frame, (640, 480))
            with self.frame_lock:
                self.color_frame = frame
        except Exception as e:
            self.get_logger().error(f'Color callback error: {e}')
    
    def depth_callback(self, msg):
        try:
            # Convert depth to viewable format
            depth = self.bridge.imgmsg_to_cv2(msg, desired_encoding='passthrough')
            # Normalize and colorize
            depth_normalized = cv2.normalize(depth, None, 0, 255, cv2.NORM_MINMAX)
            depth_8bit = depth_normalized.astype('uint8')
            frame = cv2.applyColorMap(depth_8bit, cv2.COLORMAP_JET)
            # Resize for faster streaming
            frame = cv2.resize(frame, (640, 480))
            with self.frame_lock:
                self.depth_frame = frame
        except Exception as e:
            self.get_logger().error(f'Depth callback error: {e}')
    
    def joint_callback(self, msg):
        self.joint_states = {
            'names': list(msg.name),
            'positions': [float(p) * 180.0 / 3.14159 for p in msg.position],  # Convert to degrees
            'velocities': [float(v) for v in msg.velocity] if msg.velocity else []
        }

# Global ROS bridge
ros_bridge = None

def ros_spin():
    rclpy.spin(ros_bridge)

# Flask routes
@app.route('/')
def index():
    return render_template('index.html')

def generate_frames(camera_type='color'):
    """Generator function for video streaming"""
    while True:
        with ros_bridge.frame_lock:
            if camera_type == 'color' and ros_bridge.color_frame is not None:
                frame = ros_bridge.color_frame.copy()
            elif camera_type == 'depth' and ros_bridge.depth_frame is not None:
                frame = ros_bridge.depth_frame.copy()
            else:
                # Create a black placeholder frame
                frame = np.zeros((480, 640, 3), dtype=np.uint8)
                cv2.putText(frame, f'Waiting for {camera_type} stream...', 
                          (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
        
        # Encode as JPEG with compression
        ret, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        frame_bytes = buffer.tobytes()
        
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')

@app.route('/video_feed/color')
def video_feed_color():
    return Response(generate_frames('color'),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/video_feed/depth')
def video_feed_depth():
    return Response(generate_frames('depth'),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/api/joint_states')
def get_joint_states():
    if ros_bridge.joint_states:
        return jsonify(ros_bridge.joint_states)
    return jsonify({'error': 'No data'}), 404

@app.route('/api/status')
def get_status():
    return jsonify({
        'color_active': ros_bridge.color_frame is not None,
        'depth_active': ros_bridge.depth_frame is not None,
        'joints_active': ros_bridge.joint_states is not None
    })

def main(args=None):
    global ros_bridge
    
    # Initialize ROS
    rclpy.init(args=args)
    ros_bridge = ROSBridge()
    
    # Start ROS spinning in separate thread
    ros_thread = threading.Thread(target=ros_spin, daemon=True)
    ros_thread.start()
    
    print("\n" + "="*60)
    print("🤖 Robot Web Interface Starting...")
    print("="*60)
    print(f"📡 Access the dashboard at: http://192.168.1.224:5000")
    print(f"   (Replace with your Pi's IP if different)")
    print("="*60 + "\n")
    
    # Start Flask (use threaded=True for better performance)
    try:
        app.run(host='0.0.0.0', port=5000, debug=False, threaded=True)
    except KeyboardInterrupt:
        print("\nShutting down...")
    finally:
        ros_bridge.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
