// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "crp_msgs/msg/detail/scenario__rosidl_typesupport_introspection_c.h"
#include "crp_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "crp_msgs/msg/detail/scenario__functions.h"
#include "crp_msgs/msg/detail/scenario__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `local_moving_objects`
// Member `local_obstacles`
#include "autoware_perception_msgs/msg/predicted_object.h"
// Member `local_moving_objects`
// Member `local_obstacles`
#include "autoware_perception_msgs/msg/detail/predicted_object__rosidl_typesupport_introspection_c.h"
// Member `paths`
#include "crp_msgs/msg/path_with_traffic_rules.h"
// Member `paths`
#include "crp_msgs/msg/detail/path_with_traffic_rules__rosidl_typesupport_introspection_c.h"
// Member `free_space`
#include "nav_msgs/msg/occupancy_grid.h"
// Member `free_space`
#include "nav_msgs/msg/detail/occupancy_grid__rosidl_typesupport_introspection_c.h"
// Member `maximum_speed`
#include "std_msgs/msg/float32.h"
// Member `maximum_speed`
#include "std_msgs/msg/detail/float32__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  crp_msgs__msg__Scenario__init(message_memory);
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_fini_function(void * message_memory)
{
  crp_msgs__msg__Scenario__fini(message_memory);
}

size_t crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__size_function__Scenario__local_moving_objects(
  const void * untyped_member)
{
  const autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (const autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  return member->size;
}

const void * crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__local_moving_objects(
  const void * untyped_member, size_t index)
{
  const autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (const autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  return &member->data[index];
}

void * crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__local_moving_objects(
  void * untyped_member, size_t index)
{
  autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  return &member->data[index];
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__fetch_function__Scenario__local_moving_objects(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const autoware_perception_msgs__msg__PredictedObject * item =
    ((const autoware_perception_msgs__msg__PredictedObject *)
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__local_moving_objects(untyped_member, index));
  autoware_perception_msgs__msg__PredictedObject * value =
    (autoware_perception_msgs__msg__PredictedObject *)(untyped_value);
  *value = *item;
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__assign_function__Scenario__local_moving_objects(
  void * untyped_member, size_t index, const void * untyped_value)
{
  autoware_perception_msgs__msg__PredictedObject * item =
    ((autoware_perception_msgs__msg__PredictedObject *)
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__local_moving_objects(untyped_member, index));
  const autoware_perception_msgs__msg__PredictedObject * value =
    (const autoware_perception_msgs__msg__PredictedObject *)(untyped_value);
  *item = *value;
}

bool crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__resize_function__Scenario__local_moving_objects(
  void * untyped_member, size_t size)
{
  autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  autoware_perception_msgs__msg__PredictedObject__Sequence__fini(member);
  return autoware_perception_msgs__msg__PredictedObject__Sequence__init(member, size);
}

size_t crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__size_function__Scenario__local_obstacles(
  const void * untyped_member)
{
  const autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (const autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  return member->size;
}

const void * crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__local_obstacles(
  const void * untyped_member, size_t index)
{
  const autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (const autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  return &member->data[index];
}

void * crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__local_obstacles(
  void * untyped_member, size_t index)
{
  autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  return &member->data[index];
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__fetch_function__Scenario__local_obstacles(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const autoware_perception_msgs__msg__PredictedObject * item =
    ((const autoware_perception_msgs__msg__PredictedObject *)
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__local_obstacles(untyped_member, index));
  autoware_perception_msgs__msg__PredictedObject * value =
    (autoware_perception_msgs__msg__PredictedObject *)(untyped_value);
  *value = *item;
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__assign_function__Scenario__local_obstacles(
  void * untyped_member, size_t index, const void * untyped_value)
{
  autoware_perception_msgs__msg__PredictedObject * item =
    ((autoware_perception_msgs__msg__PredictedObject *)
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__local_obstacles(untyped_member, index));
  const autoware_perception_msgs__msg__PredictedObject * value =
    (const autoware_perception_msgs__msg__PredictedObject *)(untyped_value);
  *item = *value;
}

bool crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__resize_function__Scenario__local_obstacles(
  void * untyped_member, size_t size)
{
  autoware_perception_msgs__msg__PredictedObject__Sequence * member =
    (autoware_perception_msgs__msg__PredictedObject__Sequence *)(untyped_member);
  autoware_perception_msgs__msg__PredictedObject__Sequence__fini(member);
  return autoware_perception_msgs__msg__PredictedObject__Sequence__init(member, size);
}

size_t crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__size_function__Scenario__paths(
  const void * untyped_member)
{
  const crp_msgs__msg__PathWithTrafficRules__Sequence * member =
    (const crp_msgs__msg__PathWithTrafficRules__Sequence *)(untyped_member);
  return member->size;
}

const void * crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__paths(
  const void * untyped_member, size_t index)
{
  const crp_msgs__msg__PathWithTrafficRules__Sequence * member =
    (const crp_msgs__msg__PathWithTrafficRules__Sequence *)(untyped_member);
  return &member->data[index];
}

void * crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__paths(
  void * untyped_member, size_t index)
{
  crp_msgs__msg__PathWithTrafficRules__Sequence * member =
    (crp_msgs__msg__PathWithTrafficRules__Sequence *)(untyped_member);
  return &member->data[index];
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__fetch_function__Scenario__paths(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const crp_msgs__msg__PathWithTrafficRules * item =
    ((const crp_msgs__msg__PathWithTrafficRules *)
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__paths(untyped_member, index));
  crp_msgs__msg__PathWithTrafficRules * value =
    (crp_msgs__msg__PathWithTrafficRules *)(untyped_value);
  *value = *item;
}

void crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__assign_function__Scenario__paths(
  void * untyped_member, size_t index, const void * untyped_value)
{
  crp_msgs__msg__PathWithTrafficRules * item =
    ((crp_msgs__msg__PathWithTrafficRules *)
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__paths(untyped_member, index));
  const crp_msgs__msg__PathWithTrafficRules * value =
    (const crp_msgs__msg__PathWithTrafficRules *)(untyped_value);
  *item = *value;
}

bool crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__resize_function__Scenario__paths(
  void * untyped_member, size_t size)
{
  crp_msgs__msg__PathWithTrafficRules__Sequence * member =
    (crp_msgs__msg__PathWithTrafficRules__Sequence *)(untyped_member);
  crp_msgs__msg__PathWithTrafficRules__Sequence__fini(member);
  return crp_msgs__msg__PathWithTrafficRules__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[6] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__Scenario, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "local_moving_objects",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__Scenario, local_moving_objects),  // bytes offset in struct
    NULL,  // default value
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__size_function__Scenario__local_moving_objects,  // size() function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__local_moving_objects,  // get_const(index) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__local_moving_objects,  // get(index) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__fetch_function__Scenario__local_moving_objects,  // fetch(index, &value) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__assign_function__Scenario__local_moving_objects,  // assign(index, value) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__resize_function__Scenario__local_moving_objects  // resize(index) function pointer
  },
  {
    "local_obstacles",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__Scenario, local_obstacles),  // bytes offset in struct
    NULL,  // default value
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__size_function__Scenario__local_obstacles,  // size() function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__local_obstacles,  // get_const(index) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__local_obstacles,  // get(index) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__fetch_function__Scenario__local_obstacles,  // fetch(index, &value) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__assign_function__Scenario__local_obstacles,  // assign(index, value) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__resize_function__Scenario__local_obstacles  // resize(index) function pointer
  },
  {
    "paths",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__Scenario, paths),  // bytes offset in struct
    NULL,  // default value
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__size_function__Scenario__paths,  // size() function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_const_function__Scenario__paths,  // get_const(index) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__get_function__Scenario__paths,  // get(index) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__fetch_function__Scenario__paths,  // fetch(index, &value) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__assign_function__Scenario__paths,  // assign(index, value) function pointer
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__resize_function__Scenario__paths  // resize(index) function pointer
  },
  {
    "free_space",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__Scenario, free_space),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "maximum_speed",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs__msg__Scenario, maximum_speed),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_members = {
  "crp_msgs__msg",  // message namespace
  "Scenario",  // message name
  6,  // number of fields
  sizeof(crp_msgs__msg__Scenario),
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array,  // message members
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_init_function,  // function to initialize message memory (memory has to be allocated)
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_type_support_handle = {
  0,
  &crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_crp_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, crp_msgs, msg, Scenario)() {
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, autoware_perception_msgs, msg, PredictedObject)();
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, autoware_perception_msgs, msg, PredictedObject)();
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, crp_msgs, msg, PathWithTrafficRules)();
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[4].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nav_msgs, msg, OccupancyGrid)();
  crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_member_array[5].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Float32)();
  if (!crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_type_support_handle.typesupport_identifier) {
    crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &crp_msgs__msg__Scenario__rosidl_typesupport_introspection_c__Scenario_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
