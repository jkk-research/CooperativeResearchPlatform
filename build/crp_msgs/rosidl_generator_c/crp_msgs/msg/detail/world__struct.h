// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/World.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__WORLD__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__WORLD__STRUCT_H_

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

/// Struct defined in msg/World in the package crp_msgs.
typedef struct crp_msgs__msg__World
{
  std_msgs__msg__Header header;
} crp_msgs__msg__World;

// Struct for a sequence of crp_msgs__msg__World.
typedef struct crp_msgs__msg__World__Sequence
{
  crp_msgs__msg__World * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__World__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__WORLD__STRUCT_H_
