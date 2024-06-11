// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/TrafficRule.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__STRUCT_H_

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
// Member 'stop_pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__struct.h"

/// Struct defined in msg/TrafficRule in the package crp_msgs.
typedef struct crp_msgs__msg__TrafficRule
{
  std_msgs__msg__Header header;
  /// TODO: Define constants for lane edge type (e.g. uint8 type_name=0)
  geometry_msgs__msg__PoseWithCovariance stop_pose;
  uint8_t lane_edge_type_left;
  uint8_t lane_edge_type_right;
  float maximum_speed;
} crp_msgs__msg__TrafficRule;

// Struct for a sequence of crp_msgs__msg__TrafficRule.
typedef struct crp_msgs__msg__TrafficRule__Sequence
{
  crp_msgs__msg__TrafficRule * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__TrafficRule__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__STRUCT_H_
