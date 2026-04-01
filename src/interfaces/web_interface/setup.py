from setuptools import setup
import os
from glob import glob

package_name = 'web_interface'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'templates'), glob('templates/*.html')),
        (os.path.join('share', package_name, 'static'), glob('static/*')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='bot',
    maintainer_email='you@example.com',
    description='Web interface for robot control',
    license='Apache License 2.0',
    entry_points={
        'console_scripts': [
            'flask_server = web_interface.flask_server:main',
        ],
    },
)
