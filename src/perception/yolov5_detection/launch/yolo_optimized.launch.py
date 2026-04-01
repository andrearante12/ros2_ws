from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='yolov5_detection',
            executable='yolo_detector',
            name='yolo_detector',
            parameters=[{
                'confidence_threshold': 0.5,
                'nms_threshold': 0.4,
                'image_topic': '/camera/camera/color/image_raw',
                'depth_topic': '/camera/camera/aligned_depth_to_color/image_raw',
                'publish_visualization': True,
                'frame_skip': 2  # Process every 3rd frame
            }],
            output='screen'
        )
    ])
