from setuptools import find_packages, setup

package_name = "esp32_viz"

setup(
    name=package_name,
    version="0.0.1",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="bot",
    maintainer_email="andrearante12@gmail.com",
    description="RViz visualization for ESP32 wearable controller target point",
    license="MIT",
    entry_points={
        "console_scripts": [
            "esp32_viz_node = esp32_viz.esp32_viz_node:main",
        ],
    },
)
