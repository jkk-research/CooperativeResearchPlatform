// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/PathWithTrafficRules.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'path'
#include "tier4_planning_msgs/msg/detail/path_with_lane_id__struct.h"
// Member 'traffic_rules'
#include "crp_msgs/msg/detail/traffic_rule__struct.h"

/// Struct defined in msg/PathWithTrafficRules in the package crp_msgs.
typedef struct crp_msgs__msg__PathWithTrafficRules
{
  std_msgs__msg__Header header;
  tier4_planning_msgs__msg__PathWithLaneId path;
  crp_msgs__msg__TrafficRule__Sequence traffic_rules;
} crp_msgs__msg__PathWithTrafficRules;

// Struct for a sequence of crp_msgs__msg__PathWithTrafficRules.
typedef struct crp_msgs__msg__PathWithTrafficRules__Sequence
{
  crp_msgs__msg__PathWithTrafficRules * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__PathWithTrafficRules__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__STRUCT_H_
