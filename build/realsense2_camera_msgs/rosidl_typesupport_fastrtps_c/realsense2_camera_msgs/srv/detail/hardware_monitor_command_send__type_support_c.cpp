// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from realsense2_camera_msgs:srv/HardwareMonitorCommandSend.idl
// generated code does not contain a copyright notice
#include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <cstddef>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "realsense2_camera_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__struct.h"
#include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/primitives_sequence.h"  // data
#include "rosidl_runtime_c/primitives_sequence_functions.h"  // data

// forward declare type support functions


using _HardwareMonitorCommandSend_Request__ros_msg_type = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: opcode
  {
    cdr << ros_message->opcode;
  }

  // Field name: param1
  {
    cdr << ros_message->param1;
  }

  // Field name: param2
  {
    cdr << ros_message->param2;
  }

  // Field name: param3
  {
    cdr << ros_message->param3;
  }

  // Field name: param4
  {
    cdr << ros_message->param4;
  }

  // Field name: data
  {
    size_t size = ros_message->data.size;
    auto array_ptr = ros_message->data.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  eprosima::fastcdr::Cdr & cdr,
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message)
{
  // Field name: opcode
  {
    cdr >> ros_message->opcode;
  }

  // Field name: param1
  {
    cdr >> ros_message->param1;
  }

  // Field name: param2
  {
    cdr >> ros_message->param2;
  }

  // Field name: param3
  {
    cdr >> ros_message->param3;
  }

  // Field name: param4
  {
    cdr >> ros_message->param4;
  }

  // Field name: data
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->data.data) {
      rosidl_runtime_c__uint8__Sequence__fini(&ros_message->data);
    }
    if (!rosidl_runtime_c__uint8__Sequence__init(&ros_message->data, size)) {
      fprintf(stderr, "failed to create array for field 'data'");
      return false;
    }
    auto array_ptr = ros_message->data.data;
    cdr.deserialize_array(array_ptr, size);
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _HardwareMonitorCommandSend_Request__ros_msg_type * ros_message = static_cast<const _HardwareMonitorCommandSend_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: opcode
  {
    size_t item_size = sizeof(ros_message->opcode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param1
  {
    size_t item_size = sizeof(ros_message->param1);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param2
  {
    size_t item_size = sizeof(ros_message->param2);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param3
  {
    size_t item_size = sizeof(ros_message->param3);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param4
  {
    size_t item_size = sizeof(ros_message->param4);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: data
  {
    size_t array_size = ros_message->data.size;
    auto array_ptr = ros_message->data.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: opcode
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param1
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param2
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param3
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param4
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: data
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request;
    is_plain =
      (
      offsetof(DataType, data) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: opcode
  {
    cdr << ros_message->opcode;
  }

  // Field name: param1
  {
    cdr << ros_message->param1;
  }

  // Field name: param2
  {
    cdr << ros_message->param2;
  }

  // Field name: param3
  {
    cdr << ros_message->param3;
  }

  // Field name: param4
  {
    cdr << ros_message->param4;
  }

  // Field name: data
  {
    size_t size = ros_message->data.size;
    auto array_ptr = ros_message->data.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _HardwareMonitorCommandSend_Request__ros_msg_type * ros_message = static_cast<const _HardwareMonitorCommandSend_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: opcode
  {
    size_t item_size = sizeof(ros_message->opcode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param1
  {
    size_t item_size = sizeof(ros_message->param1);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param2
  {
    size_t item_size = sizeof(ros_message->param2);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param3
  {
    size_t item_size = sizeof(ros_message->param3);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: param4
  {
    size_t item_size = sizeof(ros_message->param4);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: data
  {
    size_t array_size = ros_message->data.size;
    auto array_ptr = ros_message->data.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: opcode
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param1
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param2
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param3
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: param4
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: data
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request;
    is_plain =
      (
      offsetof(DataType, data) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _HardwareMonitorCommandSend_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message = static_cast<const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(ros_message, cdr);
}

static bool _HardwareMonitorCommandSend_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message = static_cast<realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(cdr, ros_message);
}

static uint32_t _HardwareMonitorCommandSend_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
      untyped_ros_message, 0));
}

static size_t _HardwareMonitorCommandSend_Request__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_HardwareMonitorCommandSend_Request = {
  "realsense2_camera_msgs::srv",
  "HardwareMonitorCommandSend_Request",
  _HardwareMonitorCommandSend_Request__cdr_serialize,
  _HardwareMonitorCommandSend_Request__cdr_deserialize,
  _HardwareMonitorCommandSend_Request__get_serialized_size,
  _HardwareMonitorCommandSend_Request__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _HardwareMonitorCommandSend_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_HardwareMonitorCommandSend_Request,
  get_message_typesupport_handle_function,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_hash,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_description,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Request)() {
  return &_HardwareMonitorCommandSend_Request__type_support;
}

#if defined(__cplusplus)
}
#endif

// already included above
// #include <cassert>
// already included above
// #include <cstddef>
// already included above
// #include <limits>
// already included above
// #include <string>
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
// already included above
// #include "realsense2_camera_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__struct.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__functions.h"
// already included above
// #include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

// already included above
// #include "rosidl_runtime_c/primitives_sequence.h"  // result
// already included above
// #include "rosidl_runtime_c/primitives_sequence_functions.h"  // result
#include "rosidl_runtime_c/string.h"  // error_message
#include "rosidl_runtime_c/string_functions.h"  // error_message

// forward declare type support functions


using _HardwareMonitorCommandSend_Response__ros_msg_type = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: success
  {
    cdr << (ros_message->success ? true : false);
  }

  // Field name: result
  {
    size_t size = ros_message->result.size;
    auto array_ptr = ros_message->result.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: error_message
  {
    const rosidl_runtime_c__String * str = &ros_message->error_message;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  eprosima::fastcdr::Cdr & cdr,
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message)
{
  // Field name: success
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->success = tmp ? true : false;
  }

  // Field name: result
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->result.data) {
      rosidl_runtime_c__uint8__Sequence__fini(&ros_message->result);
    }
    if (!rosidl_runtime_c__uint8__Sequence__init(&ros_message->result, size)) {
      fprintf(stderr, "failed to create array for field 'result'");
      return false;
    }
    auto array_ptr = ros_message->result.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: error_message
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->error_message.data) {
      rosidl_runtime_c__String__init(&ros_message->error_message);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->error_message,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'error_message'\n");
      return false;
    }
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _HardwareMonitorCommandSend_Response__ros_msg_type * ros_message = static_cast<const _HardwareMonitorCommandSend_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: success
  {
    size_t item_size = sizeof(ros_message->success);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: result
  {
    size_t array_size = ros_message->result.size;
    auto array_ptr = ros_message->result.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: error_message
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->error_message.size + 1);

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: success
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Field name: result
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Field name: error_message
  {
    size_t array_size = 1;
    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response;
    is_plain =
      (
      offsetof(DataType, error_message) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: success
  {
    cdr << (ros_message->success ? true : false);
  }

  // Field name: result
  {
    size_t size = ros_message->result.size;
    auto array_ptr = ros_message->result.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: error_message
  {
    const rosidl_runtime_c__String * str = &ros_message->error_message;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _HardwareMonitorCommandSend_Response__ros_msg_type * ros_message = static_cast<const _HardwareMonitorCommandSend_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: success
  {
    size_t item_size = sizeof(ros_message->success);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: result
  {
    size_t array_size = ros_message->result.size;
    auto array_ptr = ros_message->result.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: error_message
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->error_message.size + 1);

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: success
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Field name: result
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Field name: error_message
  {
    size_t array_size = 1;
    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response;
    is_plain =
      (
      offsetof(DataType, error_message) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _HardwareMonitorCommandSend_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message = static_cast<const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(ros_message, cdr);
}

static bool _HardwareMonitorCommandSend_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message = static_cast<realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(cdr, ros_message);
}

static uint32_t _HardwareMonitorCommandSend_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
      untyped_ros_message, 0));
}

static size_t _HardwareMonitorCommandSend_Response__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_HardwareMonitorCommandSend_Response = {
  "realsense2_camera_msgs::srv",
  "HardwareMonitorCommandSend_Response",
  _HardwareMonitorCommandSend_Response__cdr_serialize,
  _HardwareMonitorCommandSend_Response__cdr_deserialize,
  _HardwareMonitorCommandSend_Response__get_serialized_size,
  _HardwareMonitorCommandSend_Response__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _HardwareMonitorCommandSend_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_HardwareMonitorCommandSend_Response,
  get_message_typesupport_handle_function,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_hash,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_description,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Response)() {
  return &_HardwareMonitorCommandSend_Response__type_support;
}

#if defined(__cplusplus)
}
#endif

// already included above
// #include <cassert>
// already included above
// #include <cstddef>
// already included above
// #include <limits>
// already included above
// #include <string>
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
// already included above
// #include "realsense2_camera_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__struct.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__functions.h"
// already included above
// #include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "service_msgs/msg/detail/service_event_info__functions.h"  // info

// forward declare type support functions

bool cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  eprosima::fastcdr::Cdr & cdr,
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message);

size_t get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request * ros_message,
  eprosima::fastcdr::Cdr & cdr);

size_t get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Request)();

bool cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  eprosima::fastcdr::Cdr & cdr,
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message);

size_t get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response * ros_message,
  eprosima::fastcdr::Cdr & cdr);

size_t get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Response)();

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
bool cdr_serialize_service_msgs__msg__ServiceEventInfo(
  const service_msgs__msg__ServiceEventInfo * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
bool cdr_deserialize_service_msgs__msg__ServiceEventInfo(
  eprosima::fastcdr::Cdr & cdr,
  service_msgs__msg__ServiceEventInfo * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
size_t get_serialized_size_service_msgs__msg__ServiceEventInfo(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
size_t max_serialized_size_service_msgs__msg__ServiceEventInfo(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
bool cdr_serialize_key_service_msgs__msg__ServiceEventInfo(
  const service_msgs__msg__ServiceEventInfo * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
size_t get_serialized_size_key_service_msgs__msg__ServiceEventInfo(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
size_t max_serialized_size_key_service_msgs__msg__ServiceEventInfo(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_realsense2_camera_msgs
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_msgs, msg, ServiceEventInfo)();


using _HardwareMonitorCommandSend_Event__ros_msg_type = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: info
  {
    cdr_serialize_service_msgs__msg__ServiceEventInfo(
      &ros_message->info, cdr);
  }

  // Field name: request
  {
    size_t size = ros_message->request.size;
    auto array_ptr = ros_message->request.data;
    if (size > 1) {
      fprintf(stderr, "array size exceeds upper bound\n");
      return false;
    }
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
        &array_ptr[i], cdr);
    }
  }

  // Field name: response
  {
    size_t size = ros_message->response.size;
    auto array_ptr = ros_message->response.data;
    if (size > 1) {
      fprintf(stderr, "array size exceeds upper bound\n");
      return false;
    }
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
        &array_ptr[i], cdr);
    }
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  eprosima::fastcdr::Cdr & cdr,
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event * ros_message)
{
  // Field name: info
  {
    cdr_deserialize_service_msgs__msg__ServiceEventInfo(cdr, &ros_message->info);
  }

  // Field name: request
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->request.data) {
      realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__Sequence__fini(&ros_message->request);
    }
    if (!realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__Sequence__init(&ros_message->request, size)) {
      fprintf(stderr, "failed to create array for field 'request'");
      return false;
    }
    auto array_ptr = ros_message->request.data;
    for (size_t i = 0; i < size; ++i) {
      cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(cdr, &array_ptr[i]);
    }
  }

  // Field name: response
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->response.data) {
      realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__Sequence__fini(&ros_message->response);
    }
    if (!realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__Sequence__init(&ros_message->response, size)) {
      fprintf(stderr, "failed to create array for field 'response'");
      return false;
    }
    auto array_ptr = ros_message->response.data;
    for (size_t i = 0; i < size; ++i) {
      cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(cdr, &array_ptr[i]);
    }
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _HardwareMonitorCommandSend_Event__ros_msg_type * ros_message = static_cast<const _HardwareMonitorCommandSend_Event__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: info
  current_alignment += get_serialized_size_service_msgs__msg__ServiceEventInfo(
    &(ros_message->info), current_alignment);

  // Field name: request
  {
    size_t array_size = ros_message->request.size;
    auto array_ptr = ros_message->request.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
        &array_ptr[index], current_alignment);
    }
  }

  // Field name: response
  {
    size_t array_size = ros_message->response.size;
    auto array_ptr = ros_message->response.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
        &array_ptr[index], current_alignment);
    }
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: info
  {
    size_t array_size = 1;
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_service_msgs__msg__ServiceEventInfo(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: request
  {
    size_t array_size = 1;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: response
  {
    size_t array_size = 1;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event;
    is_plain =
      (
      offsetof(DataType, response) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
bool cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: info
  {
    cdr_serialize_key_service_msgs__msg__ServiceEventInfo(
      &ros_message->info, cdr);
  }

  // Field name: request
  {
    size_t size = ros_message->request.size;
    auto array_ptr = ros_message->request.data;
    if (size > 1) {
      fprintf(stderr, "array size exceeds upper bound\n");
      return false;
    }
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
        &array_ptr[i], cdr);
    }
  }

  // Field name: response
  {
    size_t size = ros_message->response.size;
    auto array_ptr = ros_message->response.data;
    if (size > 1) {
      fprintf(stderr, "array size exceeds upper bound\n");
      return false;
    }
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      cdr_serialize_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
        &array_ptr[i], cdr);
    }
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _HardwareMonitorCommandSend_Event__ros_msg_type * ros_message = static_cast<const _HardwareMonitorCommandSend_Event__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: info
  current_alignment += get_serialized_size_key_service_msgs__msg__ServiceEventInfo(
    &(ros_message->info), current_alignment);

  // Field name: request
  {
    size_t array_size = ros_message->request.size;
    auto array_ptr = ros_message->request.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
        &array_ptr[index], current_alignment);
    }
  }

  // Field name: response
  {
    size_t array_size = ros_message->response.size;
    auto array_ptr = ros_message->response.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += get_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
        &array_ptr[index], current_alignment);
    }
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_realsense2_camera_msgs
size_t max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: info
  {
    size_t array_size = 1;
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_key_service_msgs__msg__ServiceEventInfo(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: request
  {
    size_t array_size = 1;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: response
  {
    size_t array_size = 1;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_key_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event;
    is_plain =
      (
      offsetof(DataType, response) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _HardwareMonitorCommandSend_Event__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event * ros_message = static_cast<const realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(ros_message, cdr);
}

static bool _HardwareMonitorCommandSend_Event__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event * ros_message = static_cast<realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(cdr, ros_message);
}

static uint32_t _HardwareMonitorCommandSend_Event__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
      untyped_ros_message, 0));
}

static size_t _HardwareMonitorCommandSend_Event__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_HardwareMonitorCommandSend_Event = {
  "realsense2_camera_msgs::srv",
  "HardwareMonitorCommandSend_Event",
  _HardwareMonitorCommandSend_Event__cdr_serialize,
  _HardwareMonitorCommandSend_Event__cdr_deserialize,
  _HardwareMonitorCommandSend_Event__get_serialized_size,
  _HardwareMonitorCommandSend_Event__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _HardwareMonitorCommandSend_Event__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_HardwareMonitorCommandSend_Event,
  get_message_typesupport_handle_function,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_hash,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_description,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Event)() {
  return &_HardwareMonitorCommandSend_Event__type_support;
}

#if defined(__cplusplus)
}
#endif

#include "rosidl_typesupport_fastrtps_cpp/service_type_support.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "realsense2_camera_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "realsense2_camera_msgs/srv/hardware_monitor_command_send.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t HardwareMonitorCommandSend__callbacks = {
  "realsense2_camera_msgs::srv",
  "HardwareMonitorCommandSend",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend_Response)(),
};

static rosidl_service_type_support_t HardwareMonitorCommandSend__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &HardwareMonitorCommandSend__callbacks,
  get_service_typesupport_handle_function,
  &_HardwareMonitorCommandSend_Request__type_support,
  &_HardwareMonitorCommandSend_Response__type_support,
  &_HardwareMonitorCommandSend_Event__type_support,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    realsense2_camera_msgs,
    srv,
    HardwareMonitorCommandSend
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    realsense2_camera_msgs,
    srv,
    HardwareMonitorCommandSend
  ),
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_type_hash,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_type_description,
  &realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_type_description_sources,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, realsense2_camera_msgs, srv, HardwareMonitorCommandSend)() {
  return &HardwareMonitorCommandSend__handle;
}

#if defined(__cplusplus)
}
#endif
