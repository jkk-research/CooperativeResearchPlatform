// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from crp_msgs:msg/PathWithTrafficRules.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "crp_msgs/msg/detail/path_with_traffic_rules__rosidl_typesupport_introspection_c.h"
#include "crp_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "crp_msgs/msg/detail/path_with_traffic_rules__functions.h"
#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `path`
#include "tier4_planning_msgs/msg/path_with_lane_id.h"
// Member `path`
#include "tier4_planning_msgs/msg/detail/path_with_lane_id__rosidl_typesupport_introspection_c.h"
// Member `traffic_rules`
#include "crp_msgs/msg/traffic_rule.h"
// Member `traffic_rules`
#include "crp_msgs/msg/detail/traffic_rule__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  crp_msgs__msg__PathWithTrafficRules__init(message_memory);
}

void crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_fini_function(void * message_memory)
{
  crp_msgs__msg__PathWithTrafficRules__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__PathWithTrafficRules, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "path",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__PathWithTrafficRules, path),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "traffic_rules",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__PathWithTrafficRules, traffic_rules),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_members = {
  "crp_msgs__msg",  // message namespace
  "PathWithTrafficRules",  // message name
  3,  // number of fields
  sizeof(crp_msgs__msg__PathWithTrafficRules),
  crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_member_array,  // message members
  crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_init_function,  // function to initialize message memory (memory has to be allocated)
  crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_type_support_handle = {
  0,
  &crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_crp_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, crp_msgs, msg, PathWithTrafficRules)() {
  crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, tier4_planning_msgs, msg, PathWithLaneId)();
  crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, crp_msgs, msg, TrafficRule)();
  if (!crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_type_support_handle.typesupport_identifier) {
    crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &crp_msgs__msg__PathWithTrafficRules__rosidl_typesupport_introspection_c__PathWithTrafficRules_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
