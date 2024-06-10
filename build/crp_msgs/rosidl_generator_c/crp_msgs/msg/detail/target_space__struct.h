// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TARGET_SPACE__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__TARGET_SPACE__STRUCT_H_

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
// Member 'stopping_pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__struct.h"
// Member 'free_space'
#include "nav_msgs/msg/detail/occupancy_grid__struct.h"
// Member 'path'
#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.h"
// Member 'relevant_objects'
// Member 'relevant_obstacles'
#include "autoware_perception_msgs/msg/detail/predicted_object__struct.h"

/// Struct defined in msg/TargetSpace in the package crp_msgs.
typedef struct crp_msgs__msg__TargetSpace
{
  std_msgs__msg__Header header;
  geometry_msgs__msg__PoseWithCovariance stopping_pose;
  nav_msgs__msg__OccupancyGrid free_space;
  crp_msgs__msg__PathWithTrafficRules path;
  autoware_perception_msgs__msg__PredictedObject__Sequence relevant_objects;
  autoware_perception_msgs__msg__PredictedObject__Sequence relevant_obstacles;
} crp_msgs__msg__TargetSpace;

// Struct for a sequence of crp_msgs__msg__TargetSpace.
typedef struct crp_msgs__msg__TargetSpace__Sequence
{
  crp_msgs__msg__TargetSpace * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__TargetSpace__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__TARGET_SPACE__STRUCT_H_
