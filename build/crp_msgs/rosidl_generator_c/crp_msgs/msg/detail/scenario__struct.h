// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__SCENARIO__STRUCT_H_
#define CRP_MSGS__MSG__DETAIL__SCENARIO__STRUCT_H_

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
// Member 'local_moving_objects'
// Member 'local_obstacles'
#include "autoware_perception_msgs/msg/detail/predicted_object__struct.h"
// Member 'paths'
#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.h"
// Member 'free_space'
#include "nav_msgs/msg/detail/occupancy_grid__struct.h"
// Member 'maximum_speed'
#include "std_msgs/msg/detail/float32__struct.h"

/// Struct defined in msg/Scenario in the package crp_msgs.
typedef struct crp_msgs__msg__Scenario
{
  std_msgs__msg__Header header;
  autoware_perception_msgs__msg__PredictedObject__Sequence local_moving_objects;
  autoware_perception_msgs__msg__PredictedObject__Sequence local_obstacles;
  crp_msgs__msg__PathWithTrafficRules__Sequence paths;
  nav_msgs__msg__OccupancyGrid free_space;
  std_msgs__msg__Float32 maximum_speed;
} crp_msgs__msg__Scenario;

// Struct for a sequence of crp_msgs__msg__Scenario.
typedef struct crp_msgs__msg__Scenario__Sequence
{
  crp_msgs__msg__Scenario * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} crp_msgs__msg__Scenario__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__SCENARIO__STRUCT_H_
