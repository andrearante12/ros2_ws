cmake_minimum_required(VERSION 3.8)
project(move_program)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)

add_executable(move_program src/move_program.cpp)

ament_target_dependencies(move_program
  rclcpp
  moveit_ros_planning_interface
)

install(TARGETS move_program
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
