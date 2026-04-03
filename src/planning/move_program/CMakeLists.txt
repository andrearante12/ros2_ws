cmake_minimum_required(VERSION 3.8)
project(move_program)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(control_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(moveit_core REQUIRED)
find_package(moveit_ros_planning REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)

add_executable(move_program src/move_program.cpp)

target_include_directories(move_program PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(move_program
  rclcpp
  std_msgs
  moveit_ros_planning_interface
)

add_executable(move_with_simulation src/move_with_simulation.cpp)

target_include_directories(move_with_simulation PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(move_with_simulation
  rclcpp
  std_msgs
  moveit_ros_planning_interface
)

add_executable(move_server src/move_server.cpp)

target_include_directories(move_server PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(move_server
  rclcpp
  std_msgs
  geometry_msgs
  sensor_msgs
  rclcpp_action
  control_msgs
  trajectory_msgs
  moveit_core
  moveit_ros_planning
)

install(TARGETS move_program move_with_simulation move_server
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_export_include_directories(include)

ament_package()
