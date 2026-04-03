cmake_minimum_required(VERSION 3.8)
project(esp32_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(moveit_core REQUIRED)
find_package(moveit_ros_planning REQUIRED)
find_package(move_program REQUIRED)

add_executable(esp32_controller
  src/esp32_controller.cpp
)

target_include_directories(esp32_controller PUBLIC
  ${move_program_INCLUDE_DIRS}
)

ament_target_dependencies(esp32_controller
  rclcpp
  geometry_msgs
  sensor_msgs
  nav_msgs
  std_msgs
  moveit_core
  moveit_ros_planning
  move_program
)

install(TARGETS esp32_controller
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
