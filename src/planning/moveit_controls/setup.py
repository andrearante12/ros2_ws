import os
from glob import glob
from setuptools import setup

package_name = 'moveit_controls'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'trajectories'),
            glob('trajectories/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='andre',
    maintainer_email='andre@example.com',
    description='MoveIt Python control nodes',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'move_to_xyz = moveit_controls.move_to_xyz:main',
            'target_listener = moveit_controls.target_listener:main',
            'gripper_control = moveit_controls.gripper_control:main',
            'run_trajectory = moveit_controls.run_trajectory:main',
        ],
    },
)
