from setuptools import setup

package_name = 'chess_data_collector'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='bot',
    maintainer_email='bot@todo.com',
    description='Chess piece data collector',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'collect = chess_data_collector.image_collector:main',
            'auto_collect = chess_data_collector.timed_collector:main',
        ],
    },
)
