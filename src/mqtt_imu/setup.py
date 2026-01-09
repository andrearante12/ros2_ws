from setuptools import setup

package_name = "mqtt_imu"

setup(
    name=package_name,
    version="0.0.1",
    packages=[package_name],
    install_requires=["setuptools", "paho-mqtt"],
    zip_safe=True,
    maintainer="andre",
    description="MQTT to ROS2 IMU bridge",
    entry_points={
        "console_scripts": [
            "mqtt_imu_node = mqtt_imu.mqtt_imu_node:main",
        ],
    },
)

