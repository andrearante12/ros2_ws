// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from realsense2_camera_msgs:srv/ApplicationConfigRead.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "realsense2_camera_msgs/srv/application_config_read.h"


#ifndef REALSENSE2_CAMERA_MSGS__SRV__DETAIL__APPLICATION_CONFIG_READ__STRUCT_H_
#define REALSENSE2_CAMERA_MSGS__SRV__DETAIL__APPLICATION_CONFIG_READ__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/ApplicationConfigRead in the package realsense2_camera_msgs.
typedef struct realsense2_camera_msgs__srv__ApplicationConfigRead_Request
{
  uint8_t structure_needs_at_least_one_member;
} realsense2_camera_msgs__srv__ApplicationConfigRead_Request;

// Struct for a sequence of realsense2_camera_msgs__srv__ApplicationConfigRead_Request.
typedef struct realsense2_camera_msgs__srv__ApplicationConfigRead_Request__Sequence
{
  realsense2_camera_msgs__srv__ApplicationConfigRead_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} realsense2_camera_msgs__srv__ApplicationConfigRead_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'error_message'
// Member 'application_config'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/ApplicationConfigRead in the package realsense2_camera_msgs.
typedef struct realsense2_camera_msgs__srv__ApplicationConfigRead_Response
{
  bool success;
  rosidl_runtime_c__String error_message;
  rosidl_runtime_c__String application_config;
} realsense2_camera_msgs__srv__ApplicationConfigRead_Response;

// Struct for a sequence of realsense2_camera_msgs__srv__ApplicationConfigRead_Response.
typedef struct realsense2_camera_msgs__srv__ApplicationConfigRead_Response__Sequence
{
  realsense2_camera_msgs__srv__ApplicationConfigRead_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} realsense2_camera_msgs__srv__ApplicationConfigRead_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  realsense2_camera_msgs__srv__ApplicationConfigRead_Event__request__MAX_SIZE = 1
};
// response
enum
{
  realsense2_camera_msgs__srv__ApplicationConfigRead_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/ApplicationConfigRead in the package realsense2_camera_msgs.
typedef struct realsense2_camera_msgs__srv__ApplicationConfigRead_Event
{
  service_msgs__msg__ServiceEventInfo info;
  realsense2_camera_msgs__srv__ApplicationConfigRead_Request__Sequence request;
  realsense2_camera_msgs__srv__ApplicationConfigRead_Response__Sequence response;
} realsense2_camera_msgs__srv__ApplicationConfigRead_Event;

// Struct for a sequence of realsense2_camera_msgs__srv__ApplicationConfigRead_Event.
typedef struct realsense2_camera_msgs__srv__ApplicationConfigRead_Event__Sequence
{
  realsense2_camera_msgs__srv__ApplicationConfigRead_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} realsense2_camera_msgs__srv__ApplicationConfigRead_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // REALSENSE2_CAMERA_MSGS__SRV__DETAIL__APPLICATION_CONFIG_READ__STRUCT_H_
