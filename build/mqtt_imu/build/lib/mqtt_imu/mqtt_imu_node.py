#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from geometry_msgs.msg import Quaternion
from tf_transformations import quaternion_from_euler
import math

import paho.mqtt.client as mqtt
import json

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "esp32/dual_sensors"  # Changed to dual sensors topic


class MqttDualSensorNode(Node):

    def __init__(self):
        super().__init__("mqtt_dual_sensor_node")

        # Publisher for odometry data (from OTOS)
        self.odom_publisher_ = self.create_publisher(Odometry, "/odom", 10)
        
        # Publisher for IMU data (from MPU6050)
        self.imu_publisher_ = self.create_publisher(Imu, "/imu/data", 10)

        self.get_logger().info("Starting MQTT Dual Sensor bridge (OTOS + MPU6050)")

        # MQTT client
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.client.connect(MQTT_BROKER, MQTT_PORT, 60)
        self.client.loop_start()

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.get_logger().info("Connected to MQTT broker")
            client.subscribe(MQTT_TOPIC)
            self.get_logger().info(f"Subscribed to {MQTT_TOPIC}")
        else:
            self.get_logger().error(f"MQTT connection failed (rc={rc})")

    def on_message(self, client, userdata, msg):
        try:
            data = json.loads(msg.payload.decode())
            
            # Print received data to console
            self.get_logger().info("="*60)
            self.get_logger().info(f"Raw MQTT data: {data}")
            
            # ========================================
            # Parse OTOS position data
            # ========================================
            otos_x_inches = float(data["otos"]["x"])
            otos_y_inches = float(data["otos"]["y"])
            otos_heading_deg = float(data["otos"]["h"])
            
            # Convert inches to meters (ROS standard)
            otos_x_meters = otos_x_inches * 0.0254
            otos_y_meters = otos_y_inches * 0.0254
            otos_heading_rad = math.radians(otos_heading_deg)
            
            self.get_logger().info(f"OTOS Position: X={otos_x_inches:.2f}\" ({otos_x_meters:.3f}m), "
                                   f"Y={otos_y_inches:.2f}\" ({otos_y_meters:.3f}m), "
                                   f"Heading={otos_heading_deg:.1f}°")
            
            # ========================================
            # Parse MPU6050 orientation data
            # ========================================
            roll_deg = float(data["orient"]["r"])
            pitch_deg = float(data["orient"]["p"])
            yaw_deg = float(data["orient"]["y"])
            
            roll_rad = math.radians(roll_deg)
            pitch_rad = math.radians(pitch_deg)
            yaw_rad = math.radians(yaw_deg)
            
            self.get_logger().info(f"Orientation: Roll={roll_deg:.1f}°, "
                                   f"Pitch={pitch_deg:.1f}°, Yaw={yaw_deg:.1f}°")
            
            # ========================================
            # Parse MPU6050 accelerometer data
            # ========================================
            accel_x = float(data["accel"]["x"])
            accel_y = float(data["accel"]["y"])
            accel_z = float(data["accel"]["z"])
            
            self.get_logger().info(f"Accel: X={accel_x:.2f}, Y={accel_y:.2f}, Z={accel_z:.2f} m/s²")
            
            # ========================================
            # Parse MPU6050 gyroscope data
            # ========================================
            gyro_x = float(data["gyro"]["x"])
            gyro_y = float(data["gyro"]["y"])
            gyro_z = float(data["gyro"]["z"])
            
            self.get_logger().info(f"Gyro: X={gyro_x:.2f}, Y={gyro_y:.2f}, Z={gyro_z:.2f} rad/s")
            
            # ========================================
            # Create and publish Odometry message (OTOS data)
            # ========================================
            odom_msg = Odometry()
            odom_msg.header.stamp = self.get_clock().now().to_msg()
            odom_msg.header.frame_id = "odom"
            odom_msg.child_frame_id = "base_link"

            # Position (from OTOS, in meters)
            odom_msg.pose.pose.position.x = otos_x_meters
            odom_msg.pose.pose.position.y = otos_y_meters
            odom_msg.pose.pose.position.z = 0.0

            # Orientation (use full 3D orientation from IMU + OTOS)
            quaternion = quaternion_from_euler(roll_rad, pitch_rad, yaw_rad)
            odom_msg.pose.pose.orientation.x = quaternion[0]
            odom_msg.pose.pose.orientation.y = quaternion[1]
            odom_msg.pose.pose.orientation.z = quaternion[2]
            odom_msg.pose.pose.orientation.w = quaternion[3]

            # Covariance
            odom_msg.pose.covariance[0] = 0.01   # x variance
            odom_msg.pose.covariance[7] = 0.01   # y variance
            odom_msg.pose.covariance[35] = 0.05  # yaw variance

            # Publish odometry
            self.odom_publisher_.publish(odom_msg)
            self.get_logger().info("✓ Published Odometry message")
            
            # ========================================
            # Create and publish IMU message (MPU6050 data)
            # ========================================
            imu_msg = Imu()
            imu_msg.header.stamp = self.get_clock().now().to_msg()
            imu_msg.header.frame_id = "imu_link"
            
            # Orientation (from IMU)
            imu_msg.orientation.x = quaternion[0]
            imu_msg.orientation.y = quaternion[1]
            imu_msg.orientation.z = quaternion[2]
            imu_msg.orientation.w = quaternion[3]
            
            # Angular velocity (from gyroscope)
            imu_msg.angular_velocity.x = gyro_x
            imu_msg.angular_velocity.y = gyro_y
            imu_msg.angular_velocity.z = gyro_z
            
            # Linear acceleration (from accelerometer)
            imu_msg.linear_acceleration.x = accel_x
            imu_msg.linear_acceleration.y = accel_y
            imu_msg.linear_acceleration.z = accel_z
            
            # Covariances (can be tuned)
            imu_msg.orientation_covariance[0] = 0.01
            imu_msg.angular_velocity_covariance[0] = 0.01
            imu_msg.linear_acceleration_covariance[0] = 0.01
            
            # Publish IMU
            self.imu_publisher_.publish(imu_msg)
            self.get_logger().info("✓ Published IMU message")

        except KeyError as e:
            self.get_logger().error(f"Missing key in MQTT data: {e}")
            self.get_logger().error(f"Received data: {msg.payload.decode()}")
        except Exception as e:
            self.get_logger().error(f"Failed to parse MQTT sensor data: {e}")
            self.get_logger().error(f"Raw payload: {msg.payload.decode()}")


def main():
    rclpy.init()
    node = MqttDualSensorNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.client.loop_stop()
        node.client.disconnect()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()