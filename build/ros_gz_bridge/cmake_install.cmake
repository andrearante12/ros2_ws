# Install script for directory: /home/andre/ros2_ws/src/external/ros_gz/ros_gz_bridge

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/andre/ros2_ws/install/ros_gz_bridge")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/pythonpath.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/pythonpath.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.13/site-packages/ros_gz_bridge-1.0.18-py3.13.egg-info" TYPE DIRECTORY FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_python/ros_gz_bridge/ros_gz_bridge.egg-info/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.13/site-packages/ros_gz_bridge" TYPE DIRECTORY FILES "/home/andre/ros2_ws/src/external/ros_gz/ros_gz_bridge/ros_gz_bridge/" REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(
        COMMAND
        "/home/andre/miniconda3/bin/python3" "-m" "compileall"
        "/home/andre/ros2_ws/install/ros_gz_bridge/lib/python3.13/site-packages/ros_gz_bridge"
      )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/bridge_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node"
         OLD_RPATH "/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_node")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/bridge_node.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/andre/ros2_ws/build/ros_gz_bridge/libros_gz_bridge.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/home/andre/miniconda3/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros_gz_bridge.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros_gz_bridge" TYPE DIRECTORY FILES "/home/andre/ros2_ws/src/external/ros_gz/ros_gz_bridge/include/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/launch" TYPE DIRECTORY FILES "/home/andre/ros2_ws/src/external/ros_gz/ros_gz_bridge/launch/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/parameter_bridge")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge"
         OLD_RPATH "/home/andre/ros2_ws/build/ros_gz_bridge:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/miniconda3/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/parameter_bridge")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/static_bridge")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge"
         OLD_RPATH "/home/andre/ros2_ws/build/ros_gz_bridge:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/miniconda3/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/static_bridge")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/bridge_types")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types"
         OLD_RPATH "/home/andre/ros2_ws/build/ros_gz_bridge:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/miniconda3/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/bridge_types")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_launch_action_subscriber")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_launch_action_subscriber")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_launch_action_subscriber.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_ros_publisher")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_publisher")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_ros_publisher.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_ros_subscriber")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_subscriber")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_ros_subscriber.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_gz_publisher")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_publisher")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_gz_publisher.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_gz_subscriber")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_subscriber")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_gz_subscriber.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_gz_server")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_gz_server")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_gz_server.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge" TYPE EXECUTABLE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/test_ros_client")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client"
         OLD_RPATH "/opt/ros/jazzy/opt/gz_transport_vendor/lib:/opt/ros/jazzy/opt/gz_msgs_vendor/lib:/opt/ros/jazzy/opt/gz_math_vendor/lib:/opt/ros/jazzy/opt/gz_utils_vendor/lib:/home/andre/ros2_ws/install/ros_gz_interfaces/lib:/opt/ros/jazzy/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ros_gz_bridge/test_ros_client")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/test_ros_client.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/opt/ros/jazzy/lib/python3.12/site-packages/ament_package/template/environment_hook/library_path.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/library_path.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/package_run_dependencies" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_index/share/ament_index/resource_index/package_run_dependencies/ros_gz_bridge")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/parent_prefix_path" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_index/share/ament_index/resource_index/parent_prefix_path/ros_gz_bridge")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/opt/ros/jazzy/share/ament_cmake_core/cmake/environment_hooks/environment/ament_prefix_path.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/ament_prefix_path.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/opt/ros/jazzy/share/ament_cmake_core/cmake/environment_hooks/environment/path.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/environment" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/path.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/local_setup.bash")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/local_setup.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/local_setup.zsh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/local_setup.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_environment_hooks/package.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_index/share/ament_index/resource_index/packages/ros_gz_bridge")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/rclcpp_components" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_index/share/ament_index/resource_index/rclcpp_components/ros_gz_bridge")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake/export_ros_gz_bridgeExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake/export_ros_gz_bridgeExport.cmake"
         "/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/Export/284efbeec1bded1198aa0d1f7623741f/export_ros_gz_bridgeExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake/export_ros_gz_bridgeExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake/export_ros_gz_bridgeExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/Export/284efbeec1bded1198aa0d1f7623741f/export_ros_gz_bridgeExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/CMakeFiles/Export/284efbeec1bded1198aa0d1f7623741f/export_ros_gz_bridgeExport-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_export_include_directories/ament_cmake_export_include_directories-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_export_libraries/ament_cmake_export_libraries-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_export_targets/ament_cmake_export_targets-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_export_dependencies/ament_cmake_export_dependencies-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge/cmake" TYPE FILE FILES
    "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_core/ros_gz_bridgeConfig.cmake"
    "/home/andre/ros2_ws/build/ros_gz_bridge/ament_cmake_core/ros_gz_bridgeConfig-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros_gz_bridge" TYPE FILE FILES "/home/andre/ros2_ws/src/external/ros_gz/ros_gz_bridge/package.xml")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/andre/ros2_ws/build/ros_gz_bridge/gtest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/andre/ros2_ws/build/ros_gz_bridge/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
