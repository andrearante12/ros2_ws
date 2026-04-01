// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from realsense2_camera_msgs:srv/SafetyPresetWrite.idl
// generated code does not contain a copyright notice

#include "realsense2_camera_msgs/srv/detail/safety_preset_write__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__SafetyPresetWrite__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xbe, 0x4b, 0xca, 0x13, 0x73, 0x7a, 0xb5, 0xd2,
      0xce, 0x13, 0x0c, 0x95, 0xcc, 0xd2, 0x8b, 0x78,
      0x59, 0x78, 0x83, 0x72, 0x19, 0x6b, 0x3b, 0x6c,
      0x51, 0x75, 0x5c, 0x1f, 0x62, 0x47, 0x3a, 0x70,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x8d, 0xbe, 0xb1, 0x6f, 0xf3, 0xd8, 0x51, 0xcd,
      0x88, 0xd1, 0x96, 0xf2, 0x89, 0x31, 0x06, 0x23,
      0x83, 0x0e, 0x23, 0xf1, 0x63, 0xec, 0x0c, 0x06,
      0xab, 0xd9, 0x52, 0x74, 0x15, 0x3d, 0xf2, 0xeb,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x8f, 0x46, 0x18, 0x2f, 0x62, 0xdb, 0xc4, 0xd8,
      0x50, 0x8e, 0x46, 0xe8, 0x64, 0x03, 0xd8, 0x62,
      0x11, 0xbc, 0x15, 0xc3, 0x41, 0x59, 0x5e, 0x10,
      0xf1, 0xbd, 0x1b, 0xa5, 0x2b, 0x64, 0xce, 0x51,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_realsense2_camera_msgs
const rosidl_type_hash_t *
realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xcf, 0x14, 0xd9, 0xd0, 0x6b, 0x4b, 0x11, 0xeb,
      0x49, 0x79, 0xa1, 0x29, 0x05, 0xac, 0xf5, 0x7a,
      0xea, 0xa8, 0xef, 0x92, 0x70, 0x58, 0x50, 0xd2,
      0x4d, 0xf4, 0x0f, 0xc5, 0xac, 0x96, 0xa3, 0x9f,
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

static char realsense2_camera_msgs__srv__SafetyPresetWrite__TYPE_NAME[] = "realsense2_camera_msgs/srv/SafetyPresetWrite";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Event__TYPE_NAME[] = "realsense2_camera_msgs/srv/SafetyPresetWrite_Event";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME[] = "realsense2_camera_msgs/srv/SafetyPresetWrite_Request";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME[] = "realsense2_camera_msgs/srv/SafetyPresetWrite_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__SafetyPresetWrite__FIELD_NAME__request_message[] = "request_message";
static char realsense2_camera_msgs__srv__SafetyPresetWrite__FIELD_NAME__response_message[] = "response_message";
static char realsense2_camera_msgs__srv__SafetyPresetWrite__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__SafetyPresetWrite__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME, 52, 52},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME, 53, 53},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__TYPE_NAME, 50, 50},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription realsense2_camera_msgs__srv__SafetyPresetWrite__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__TYPE_NAME, 50, 50},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME, 52, 52},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME, 53, 53},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__SafetyPresetWrite__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__SafetyPresetWrite__TYPE_NAME, 44, 44},
      {realsense2_camera_msgs__srv__SafetyPresetWrite__FIELDS, 3, 3},
    },
    {realsense2_camera_msgs__srv__SafetyPresetWrite__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Request__FIELD_NAME__safety_preset[] = "safety_preset";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Request__FIELD_NAME__index[] = "index";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__SafetyPresetWrite_Request__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__FIELD_NAME__safety_preset, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__FIELD_NAME__index, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME, 52, 52},
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Response__FIELD_NAME__success[] = "success";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Response__FIELD_NAME__error_message[] = "error_message";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__SafetyPresetWrite_Response__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__FIELD_NAME__error_message, 13, 13},
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
realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME, 53, 53},
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELD_NAME__info[] = "info";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELD_NAME__request[] = "request";
static char realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELDS[] = {
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME, 52, 52},
    },
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME, 53, 53},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription realsense2_camera_msgs__srv__SafetyPresetWrite_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME, 52, 52},
    {NULL, 0, 0},
  },
  {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME, 53, 53},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__TYPE_NAME, 50, 50},
      {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__FIELDS, 3, 3},
    },
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string safety_preset\n"
  "uint8 index\n"
  "---\n"
  "bool success\n"
  "string error_message";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__SafetyPresetWrite__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__SafetyPresetWrite__TYPE_NAME, 44, 44},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 71, 71},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Request__TYPE_NAME, 52, 52},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Response__TYPE_NAME, 53, 53},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {realsense2_camera_msgs__srv__SafetyPresetWrite_Event__TYPE_NAME, 50, 50},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__SafetyPresetWrite__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__SafetyPresetWrite__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_individual_type_description_source(NULL);
    sources[3] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_individual_type_description_source(NULL);
    sources[4] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Request__get_individual_type_description_source(NULL);
    sources[3] = *realsense2_camera_msgs__srv__SafetyPresetWrite_Response__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
