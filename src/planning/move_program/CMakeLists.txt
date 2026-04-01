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

install(TARGETS move_program
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_export_include_directories(include)

ament_package()
