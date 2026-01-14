cmake_minimum_required(VERSION 3.8)
project(esp32_controller)

# ===============================
# Compiler settings
# ===============================
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===============================
# Find dependencies
# ===============================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)

# ===============================
# Build node
# ===============================
add_executable(esp32_controller
  src/esp32_controller.cpp
)

ament_target_dependencies(esp32_controller
  rclcpp
  sensor_msgs
  nav_msgs
  moveit_ros_planning_interface
)

# ===============================
# Install
# ===============================
install(TARGETS esp32_controller
  DESTINATION lib/${PROJECT_NAME}
)

# ===============================
# Package export
# ===============================
ament_package()