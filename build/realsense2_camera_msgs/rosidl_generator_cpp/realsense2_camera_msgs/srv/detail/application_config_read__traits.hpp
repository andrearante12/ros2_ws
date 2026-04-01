// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from realsense2_camera_msgs:srv/ApplicationConfigRead.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "realsense2_camera_msgs/srv/application_config_read.hpp"


#ifndef REALSENSE2_CAMERA_MSGS__SRV__DETAIL__APPLICATION_CONFIG_READ__TRAITS_HPP_
#define REALSENSE2_CAMERA_MSGS__SRV__DETAIL__APPLICATION_CONFIG_READ__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "realsense2_camera_msgs/srv/detail/application_config_read__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace realsense2_camera_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const ApplicationConfigRead_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ApplicationConfigRead_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ApplicationConfigRead_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_generator_traits
{

[[deprecated("use realsense2_camera_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const realsense2_camera_msgs::srv::ApplicationConfigRead_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  realsense2_camera_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use realsense2_camera_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const realsense2_camera_msgs::srv::ApplicationConfigRead_Request & msg)
{
  return realsense2_camera_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>()
{
  return "realsense2_camera_msgs::srv::ApplicationConfigRead_Request";
}

template<>
inline const char * name<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>()
{
  return "realsense2_camera_msgs/srv/ApplicationConfigRead_Request";
}

template<>
struct has_fixed_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace realsense2_camera_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const ApplicationConfigRead_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: error_message
  {
    out << "error_message: ";
    rosidl_generator_traits::value_to_yaml(msg.error_message, out);
    out << ", ";
  }

  // member: application_config
  {
    out << "application_config: ";
    rosidl_generator_traits::value_to_yaml(msg.application_config, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ApplicationConfigRead_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: error_message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "error_message: ";
    rosidl_generator_traits::value_to_yaml(msg.error_message, out);
    out << "\n";
  }

  // member: application_config
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "application_config: ";
    rosidl_generator_traits::value_to_yaml(msg.application_config, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ApplicationConfigRead_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_generator_traits
{

[[deprecated("use realsense2_camera_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const realsense2_camera_msgs::srv::ApplicationConfigRead_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  realsense2_camera_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use realsense2_camera_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const realsense2_camera_msgs::srv::ApplicationConfigRead_Response & msg)
{
  return realsense2_camera_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>()
{
  return "realsense2_camera_msgs::srv::ApplicationConfigRead_Response";
}

template<>
inline const char * name<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>()
{
  return "realsense2_camera_msgs/srv/ApplicationConfigRead_Response";
}

template<>
struct has_fixed_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace realsense2_camera_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const ApplicationConfigRead_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ApplicationConfigRead_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ApplicationConfigRead_Event & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace realsense2_camera_msgs

namespace rosidl_generator_traits
{

[[deprecated("use realsense2_camera_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const realsense2_camera_msgs::srv::ApplicationConfigRead_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  realsense2_camera_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use realsense2_camera_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const realsense2_camera_msgs::srv::ApplicationConfigRead_Event & msg)
{
  return realsense2_camera_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<realsense2_camera_msgs::srv::ApplicationConfigRead_Event>()
{
  return "realsense2_camera_msgs::srv::ApplicationConfigRead_Event";
}

template<>
inline const char * name<realsense2_camera_msgs::srv::ApplicationConfigRead_Event>()
{
  return "realsense2_camera_msgs/srv/ApplicationConfigRead_Event";
}

template<>
struct has_fixed_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Event>
  : std::integral_constant<bool, has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>::value && has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>::value && has_bounded_size<service_msgs::msg::ServiceEventInfo>::value> {};

template<>
struct is_message<realsense2_camera_msgs::srv::ApplicationConfigRead_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<realsense2_camera_msgs::srv::ApplicationConfigRead>()
{
  return "realsense2_camera_msgs::srv::ApplicationConfigRead";
}

template<>
inline const char * name<realsense2_camera_msgs::srv::ApplicationConfigRead>()
{
  return "realsense2_camera_msgs/srv/ApplicationConfigRead";
}

template<>
struct has_fixed_size<realsense2_camera_msgs::srv::ApplicationConfigRead>
  : std::integral_constant<
    bool,
    has_fixed_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>::value &&
    has_fixed_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>::value
  >
{
};

template<>
struct has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead>
  : std::integral_constant<
    bool,
    has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>::value &&
    has_bounded_size<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>::value
  >
{
};

template<>
struct is_service<realsense2_camera_msgs::srv::ApplicationConfigRead>
  : std::true_type
{
};

template<>
struct is_service_request<realsense2_camera_msgs::srv::ApplicationConfigRead_Request>
  : std::true_type
{
};

template<>
struct is_service_response<realsense2_camera_msgs::srv::ApplicationConfigRead_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // REALSENSE2_CAMERA_MSGS__SRV__DETAIL__APPLICATION_CONFIG_READ__TRAITS_HPP_
