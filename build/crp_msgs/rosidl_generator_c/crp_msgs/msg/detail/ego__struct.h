// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/Ego.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__EGO__STRUCT_H_

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
// Member 'pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__struct.h"
// Member 'twist'
#include "geometry_msgs/msg/detail/twist_with_covariance__struct.h"
// Member 'accel'
#include "geometry_msgs/msg/detail/accel_with_covariance__struct.h"

/// Struct defined in msg/Ego in the package crp_msgs.
typedef struct crp_msgs__msg__Ego
{
  std_msgs__msg__Header header;
  geometry_msgs__msg__PoseWithCovariance pose;
  geometry_msgs__msg__TwistWithCovariance twist;
  geometry_msgs__msg__AccelWithCovariance accel;
  float wheel_angle;
} crp_msgs__msg__Ego;

// Struct for a sequence of crp_msgs__msg__Ego.
typedef struct crp_msgs__msg__Ego__Sequence
{
  crp_msgs__msg__Ego * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__Ego__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__EGO__STRUCT_H_
