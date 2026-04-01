// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from realsense2_camera_msgs:srv/HardwareMonitorCommandSend.idl
// generated code does not contain a copyright notice

#include "realsense2_camera_msgs/srv/detail/hardware_monitor_command_send__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x46, 0xe4, 0x76, 0x6c, 0x77, 0x16, 0xc8, 0xb1,
      0xdd, 0xb4, 0x06, 0x01, 0xd4, 0xcf, 0xf1, 0x13,
      0xdf, 0x51, 0x26, 0xa4, 0xfe, 0x87, 0x58, 0x75,
      0x88, 0xd2, 0xc5, 0x32, 0x5d, 0xb9, 0xd3, 0x78,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xad, 0x75, 0x57, 0x0e, 0x43, 0xab, 0x3e, 0xbf,
      0x8c, 0x79, 0x16, 0x8e, 0xbc, 0x0b, 0xc9, 0xc3,
      0xb5, 0x6f, 0xee, 0x05, 0x89, 0x66, 0xbb, 0x80,
      0xe7, 0xb4, 0x8e, 0x61, 0xc7, 0x1d, 0x03, 0xb7,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x31, 0x9c, 0x30, 0x56, 0x13, 0x40, 0xbf, 0x34,
      0xc0, 0x3a, 0x9a, 0x4f, 0x5b, 0x62, 0x23, 0xfc,
      0x15, 0xb9, 0x70, 0xd1, 0x66, 0xda, 0x65, 0x9e,
      0x1b, 0xab, 0x7c, 0x41, 0xcd, 0xe0, 0x2d, 0x77,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x00, 0x34, 0xa7, 0x5c, 0x61, 0x81, 0x5a, 0xbd,
      0xf9, 0xfb, 0x90, 0x47, 0xa3, 0x52, 0xa4, 0xa2,
      0x5a, 0x9d, 0x78, 0xd6, 0xed, 0xc4, 0xc7, 0xaf,
      0x1a, 0x83, 0x8b, 0x91, 0x38, 0x6f, 0x0d, 0xa8,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "service_msgs/msg/detail/service_event_info__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend__TYPE_NAME[] = "realsense2_camera_msgs/srv/HardwareMonitorCommandSend";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__TYPE_NAME[] = "realsense2_camera_msgs/srv/HardwareMonitorCommandSend_Event";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME[] = "realsense2_camera_msgs/srv/HardwareMonitorCommandSend_Request";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME[] = "realsense2_camera_msgs/srv/HardwareMonitorCommandSend_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELD_NAME__request_message[] = "request_message";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELD_NAME__response_message[] = "response_message";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME, 61, 61},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME, 62, 62},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__TYPE_NAME, 59, 59},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription realsense2_camera_msgs__srv__HardwareMonitorCommandSend__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__TYPE_NAME, 59, 59},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME, 61, 61},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME, 62, 62},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__TYPE_NAME, 53, 53},
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__FIELDS, 3, 3},
    },
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__opcode[] = "opcode";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param1[] = "param1";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param2[] = "param2";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param3[] = "param3";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param4[] = "param4";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__data[] = "data";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__opcode, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param1, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param2, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param3, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__param4, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELD_NAME__data, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME, 61, 61},
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__FIELDS, 6, 6},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELD_NAME__success[] = "success";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELD_NAME__result[] = "result";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELD_NAME__error_message[] = "error_message";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELD_NAME__result, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELD_NAME__error_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME, 62, 62},
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELD_NAME__info[] = "info";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELD_NAME__request[] = "request";
static char realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME, 61, 61},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME, 62, 62},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME, 61, 61},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME, 62, 62},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__TYPE_NAME, 59, 59},
      {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__FIELDS, 3, 3},
    },
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint32 opcode\n"
  "uint32 param1\n"
  "uint32 param2\n"
  "uint32 param3\n"
  "uint32 param4\n"
  "uint8[] data\n"
  "---\n"
  "bool success\n"
  "uint8[] result\n"
  "string error_message";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend__TYPE_NAME, 53, 53},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 136, 136},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__TYPE_NAME, 61, 61},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__TYPE_NAME, 62, 62},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__TYPE_NAME, 59, 59},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_individual_type_description_source(NULL);
    sources[3] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_individual_type_description_source(NULL);
    sources[4] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Request__get_individual_type_description_source(NULL);
    sources[3] = *realsense2_camera_msgs__srv__HardwareMonitorCommandSend_Response__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
