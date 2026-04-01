// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from realsense2_camera_msgs:srv/SafetyPresetRead.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "realsense2_camera_msgs/srv/detail/safety_preset_read__functions.h"
#include "realsense2_camera_msgs/srv/detail/safety_preset_read__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace realsense2_camera_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SafetyPresetRead_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SafetyPresetRead_Request_type_support_ids_t;

static const _SafetyPresetRead_Request_type_support_ids_t _SafetyPresetRead_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SafetyPresetRead_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SafetyPresetRead_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SafetyPresetRead_Request_type_support_symbol_names_t _SafetyPresetRead_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Request)),
  }
};

typedef struct _SafetyPresetRead_Request_type_support_data_t
{
  void * data[2];
} _SafetyPresetRead_Request_type_support_data_t;

static _SafetyPresetRead_Request_type_support_data_t _SafetyPresetRead_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SafetyPresetRead_Request_message_typesupport_map = {
  2,
  "realsense2_camera_msgs",
  &_SafetyPresetRead_Request_message_typesupport_ids.typesupport_identifier[0],
  &_SafetyPresetRead_Request_message_typesupport_symbol_names.symbol_name[0],
  &_SafetyPresetRead_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SafetyPresetRead_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SafetyPresetRead_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Request__get_type_hash,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Request__get_type_description,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Request>()
{
  return &::realsense2_camera_msgs::srv::rosidl_typesupport_cpp::SafetyPresetRead_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Request)() {
  return get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/safety_preset_read__functions.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/safety_preset_read__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace realsense2_camera_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SafetyPresetRead_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SafetyPresetRead_Response_type_support_ids_t;

static const _SafetyPresetRead_Response_type_support_ids_t _SafetyPresetRead_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SafetyPresetRead_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SafetyPresetRead_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SafetyPresetRead_Response_type_support_symbol_names_t _SafetyPresetRead_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Response)),
  }
};

typedef struct _SafetyPresetRead_Response_type_support_data_t
{
  void * data[2];
} _SafetyPresetRead_Response_type_support_data_t;

static _SafetyPresetRead_Response_type_support_data_t _SafetyPresetRead_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SafetyPresetRead_Response_message_typesupport_map = {
  2,
  "realsense2_camera_msgs",
  &_SafetyPresetRead_Response_message_typesupport_ids.typesupport_identifier[0],
  &_SafetyPresetRead_Response_message_typesupport_symbol_names.symbol_name[0],
  &_SafetyPresetRead_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SafetyPresetRead_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SafetyPresetRead_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Response__get_type_hash,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Response__get_type_description,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Response>()
{
  return &::realsense2_camera_msgs::srv::rosidl_typesupport_cpp::SafetyPresetRead_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Response)() {
  return get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/safety_preset_read__functions.h"
// already included above
// #include "realsense2_camera_msgs/srv/detail/safety_preset_read__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace realsense2_camera_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SafetyPresetRead_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SafetyPresetRead_Event_type_support_ids_t;

static const _SafetyPresetRead_Event_type_support_ids_t _SafetyPresetRead_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SafetyPresetRead_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SafetyPresetRead_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SafetyPresetRead_Event_type_support_symbol_names_t _SafetyPresetRead_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Event)),
  }
};

typedef struct _SafetyPresetRead_Event_type_support_data_t
{
  void * data[2];
} _SafetyPresetRead_Event_type_support_data_t;

static _SafetyPresetRead_Event_type_support_data_t _SafetyPresetRead_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SafetyPresetRead_Event_message_typesupport_map = {
  2,
  "realsense2_camera_msgs",
  &_SafetyPresetRead_Event_message_typesupport_ids.typesupport_identifier[0],
  &_SafetyPresetRead_Event_message_typesupport_symbol_names.symbol_name[0],
  &_SafetyPresetRead_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SafetyPresetRead_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SafetyPresetRead_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Event__get_type_hash,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Event__get_type_description,
  &realsense2_camera_msgs__srv__SafetyPresetRead_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Event>()
{
  return &::realsense2_camera_msgs::srv::rosidl_typesupport_cpp::SafetyPresetRead_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, realsense2_camera_msgs, srv, SafetyPresetRead_Event)() {
  return get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Event>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "realsense2_camera_msgs/srv/detail/safety_preset_read__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace realsense2_camera_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SafetyPresetRead_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SafetyPresetRead_type_support_ids_t;

static const _SafetyPresetRead_type_support_ids_t _SafetyPresetRead_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SafetyPresetRead_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SafetyPresetRead_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SafetyPresetRead_type_support_symbol_names_t _SafetyPresetRead_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, realsense2_camera_msgs, srv, SafetyPresetRead)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, realsense2_camera_msgs, srv, SafetyPresetRead)),
  }
};

typedef struct _SafetyPresetRead_type_support_data_t
{
  void * data[2];
} _SafetyPresetRead_type_support_data_t;

static _SafetyPresetRead_type_support_data_t _SafetyPresetRead_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SafetyPresetRead_service_typesupport_map = {
  2,
  "realsense2_camera_msgs",
  &_SafetyPresetRead_service_typesupport_ids.typesupport_identifier[0],
  &_SafetyPresetRead_service_typesupport_symbol_names.symbol_name[0],
  &_SafetyPresetRead_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t SafetyPresetRead_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SafetyPresetRead_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<realsense2_camera_msgs::srv::SafetyPresetRead>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<realsense2_camera_msgs::srv::SafetyPresetRead>,
  &realsense2_camera_msgs__srv__SafetyPresetRead__get_type_hash,
  &realsense2_camera_msgs__srv__SafetyPresetRead__get_type_description,
  &realsense2_camera_msgs__srv__SafetyPresetRead__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead>()
{
  return &::realsense2_camera_msgs::srv::rosidl_typesupport_cpp::SafetyPresetRead_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, realsense2_camera_msgs, srv, SafetyPresetRead)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<realsense2_camera_msgs::srv::SafetyPresetRead>();
}

#ifdef __cplusplus
}
#endif
