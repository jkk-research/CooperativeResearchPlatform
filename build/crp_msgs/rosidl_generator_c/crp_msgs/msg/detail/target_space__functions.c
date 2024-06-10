// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice
#include "crp_msgs/msg/detail/target_space__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `stopping_pose`
#include "geometry_msgs/msg/detail/pose_with_covariance__functions.h"
// Member `free_space`
#include "nav_msgs/msg/detail/occupancy_grid__functions.h"
// Member `path`
#include "crp_msgs/msg/detail/path_with_traffic_rules__functions.h"
// Member `relevant_objects`
// Member `relevant_obstacles`
#include "autoware_perception_msgs/msg/detail/predicted_object__functions.h"

bool
crp_msgs__msg__TargetSpace__init(crp_msgs__msg__TargetSpace * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    crp_msgs__msg__TargetSpace__fini(msg);
    return false;
  }
  // stopping_pose
  if (!geometry_msgs__msg__PoseWithCovariance__init(&msg->stopping_pose)) {
    crp_msgs__msg__TargetSpace__fini(msg);
    return false;
  }
  // free_space
  if (!nav_msgs__msg__OccupancyGrid__init(&msg->free_space)) {
    crp_msgs__msg__TargetSpace__fini(msg);
    return false;
  }
  // path
  if (!crp_msgs__msg__PathWithTrafficRules__init(&msg->path)) {
    crp_msgs__msg__TargetSpace__fini(msg);
    return false;
  }
  // relevant_objects
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__init(&msg->relevant_objects, 0)) {
    crp_msgs__msg__TargetSpace__fini(msg);
    return false;
  }
  // relevant_obstacles
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__init(&msg->relevant_obstacles, 0)) {
    crp_msgs__msg__TargetSpace__fini(msg);
    return false;
  }
  return true;
}

void
crp_msgs__msg__TargetSpace__fini(crp_msgs__msg__TargetSpace * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // stopping_pose
  geometry_msgs__msg__PoseWithCovariance__fini(&msg->stopping_pose);
  // free_space
  nav_msgs__msg__OccupancyGrid__fini(&msg->free_space);
  // path
  crp_msgs__msg__PathWithTrafficRules__fini(&msg->path);
  // relevant_objects
  autoware_perception_msgs__msg__PredictedObject__Sequence__fini(&msg->relevant_objects);
  // relevant_obstacles
  autoware_perception_msgs__msg__PredictedObject__Sequence__fini(&msg->relevant_obstacles);
}

bool
crp_msgs__msg__TargetSpace__are_equal(const crp_msgs__msg__TargetSpace * lhs, const crp_msgs__msg__TargetSpace * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // stopping_pose
  if (!geometry_msgs__msg__PoseWithCovariance__are_equal(
      &(lhs->stopping_pose), &(rhs->stopping_pose)))
  {
    return false;
  }
  // free_space
  if (!nav_msgs__msg__OccupancyGrid__are_equal(
      &(lhs->free_space), &(rhs->free_space)))
  {
    return false;
  }
  // path
  if (!crp_msgs__msg__PathWithTrafficRules__are_equal(
      &(lhs->path), &(rhs->path)))
  {
    return false;
  }
  // relevant_objects
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__are_equal(
      &(lhs->relevant_objects), &(rhs->relevant_objects)))
  {
    return false;
  }
  // relevant_obstacles
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__are_equal(
      &(lhs->relevant_obstacles), &(rhs->relevant_obstacles)))
  {
    return false;
  }
  return true;
}

bool
crp_msgs__msg__TargetSpace__copy(
  const crp_msgs__msg__TargetSpace * input,
  crp_msgs__msg__TargetSpace * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // stopping_pose
  if (!geometry_msgs__msg__PoseWithCovariance__copy(
      &(input->stopping_pose), &(output->stopping_pose)))
  {
    return false;
  }
  // free_space
  if (!nav_msgs__msg__OccupancyGrid__copy(
      &(input->free_space), &(output->free_space)))
  {
    return false;
  }
  // path
  if (!crp_msgs__msg__PathWithTrafficRules__copy(
      &(input->path), &(output->path)))
  {
    return false;
  }
  // relevant_objects
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__copy(
      &(input->relevant_objects), &(output->relevant_objects)))
  {
    return false;
  }
  // relevant_obstacles
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__copy(
      &(input->relevant_obstacles), &(output->relevant_obstacles)))
  {
    return false;
  }
  return true;
}

crp_msgs__msg__TargetSpace *
crp_msgs__msg__TargetSpace__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__TargetSpace * msg = (crp_msgs__msg__TargetSpace *)allocator.allocate(sizeof(crp_msgs__msg__TargetSpace), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(crp_msgs__msg__TargetSpace));
  bool success = crp_msgs__msg__TargetSpace__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
crp_msgs__msg__TargetSpace__destroy(crp_msgs__msg__TargetSpace * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    crp_msgs__msg__TargetSpace__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
crp_msgs__msg__TargetSpace__Sequence__init(crp_msgs__msg__TargetSpace__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__TargetSpace * data = NULL;

  if (size) {
    data = (crp_msgs__msg__TargetSpace *)allocator.zero_allocate(size, sizeof(crp_msgs__msg__TargetSpace), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = crp_msgs__msg__TargetSpace__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        crp_msgs__msg__TargetSpace__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
crp_msgs__msg__TargetSpace__Sequence__fini(crp_msgs__msg__TargetSpace__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      crp_msgs__msg__TargetSpace__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

crp_msgs__msg__TargetSpace__Sequence *
crp_msgs__msg__TargetSpace__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__TargetSpace__Sequence * array = (crp_msgs__msg__TargetSpace__Sequence *)allocator.allocate(sizeof(crp_msgs__msg__TargetSpace__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = crp_msgs__msg__TargetSpace__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
crp_msgs__msg__TargetSpace__Sequence__destroy(crp_msgs__msg__TargetSpace__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    crp_msgs__msg__TargetSpace__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
crp_msgs__msg__TargetSpace__Sequence__are_equal(const crp_msgs__msg__TargetSpace__Sequence * lhs, const crp_msgs__msg__TargetSpace__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!crp_msgs__msg__TargetSpace__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
crp_msgs__msg__TargetSpace__Sequence__copy(
  const crp_msgs__msg__TargetSpace__Sequence * input,
  crp_msgs__msg__TargetSpace__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(crp_msgs__msg__TargetSpace);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    crp_msgs__msg__TargetSpace * data =
      (crp_msgs__msg__TargetSpace *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!crp_msgs__msg__TargetSpace__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          crp_msgs__msg__TargetSpace__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!crp_msgs__msg__TargetSpace__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
