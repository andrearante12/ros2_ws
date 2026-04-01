from setuptools import find_packages
from setuptools import setup

setup(
    name='ros_gz_bridge',
    version='1.0.18',
    packages=find_packages(
        include=('ros_gz_bridge', 'ros_gz_bridge.*')),
)
