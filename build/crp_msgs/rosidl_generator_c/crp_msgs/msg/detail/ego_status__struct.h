// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/EgoStatus.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO_STATUS__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__EGO_STATUS__STRUCT_H_

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

/// Struct defined in msg/EgoStatus in the package crp_msgs.
typedef struct crp_msgs__msg__EgoStatus
{
  std_msgs__msg__Header header;
  float wheel_status;
} crp_msgs__msg__EgoStatus;

// Struct for a sequence of crp_msgs__msg__EgoStatus.
typedef struct crp_msgs__msg__EgoStatus__Sequence
{
  crp_msgs__msg__EgoStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__EgoStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__EGO_STATUS__STRUCT_H_
