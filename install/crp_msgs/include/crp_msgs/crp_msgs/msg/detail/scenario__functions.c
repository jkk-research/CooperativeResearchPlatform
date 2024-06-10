// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice
#include "crp_msgs/msg/detail/scenario__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `local_moving_objects`
// Member `local_obstacles`
#include "autoware_perception_msgs/msg/detail/predicted_object__functions.h"
// Member `paths`
#include "crp_msgs/msg/detail/path_with_traffic_rules__functions.h"
// Member `free_space`
#include "nav_msgs/msg/detail/occupancy_grid__functions.h"
// Member `maximum_speed`
#include "std_msgs/msg/detail/float32__functions.h"

bool
crp_msgs__msg__Scenario__init(crp_msgs__msg__Scenario * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    crp_msgs__msg__Scenario__fini(msg);
    return false;
  }
  // local_moving_objects
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__init(&msg->local_moving_objects, 0)) {
    crp_msgs__msg__Scenario__fini(msg);
    return false;
  }
  // local_obstacles
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__init(&msg->local_obstacles, 0)) {
    crp_msgs__msg__Scenario__fini(msg);
    return false;
  }
  // paths
  if (!crp_msgs__msg__PathWithTrafficRules__Sequence__init(&msg->paths, 0)) {
    crp_msgs__msg__Scenario__fini(msg);
    return false;
  }
  // free_space
  if (!nav_msgs__msg__OccupancyGrid__init(&msg->free_space)) {
    crp_msgs__msg__Scenario__fini(msg);
    return false;
  }
  // maximum_speed
  if (!std_msgs__msg__Float32__init(&msg->maximum_speed)) {
    crp_msgs__msg__Scenario__fini(msg);
    return false;
  }
  return true;
}

void
crp_msgs__msg__Scenario__fini(crp_msgs__msg__Scenario * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // local_moving_objects
  autoware_perception_msgs__msg__PredictedObject__Sequence__fini(&msg->local_moving_objects);
  // local_obstacles
  autoware_perception_msgs__msg__PredictedObject__Sequence__fini(&msg->local_obstacles);
  // paths
  crp_msgs__msg__PathWithTrafficRules__Sequence__fini(&msg->paths);
  // free_space
  nav_msgs__msg__OccupancyGrid__fini(&msg->free_space);
  // maximum_speed
  std_msgs__msg__Float32__fini(&msg->maximum_speed);
}

bool
crp_msgs__msg__Scenario__are_equal(const crp_msgs__msg__Scenario * lhs, const crp_msgs__msg__Scenario * rhs)
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
  // local_moving_objects
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__are_equal(
      &(lhs->local_moving_objects), &(rhs->local_moving_objects)))
  {
    return false;
  }
  // local_obstacles
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__are_equal(
      &(lhs->local_obstacles), &(rhs->local_obstacles)))
  {
    return false;
  }
  // paths
  if (!crp_msgs__msg__PathWithTrafficRules__Sequence__are_equal(
      &(lhs->paths), &(rhs->paths)))
  {
    return false;
  }
  // free_space
  if (!nav_msgs__msg__OccupancyGrid__are_equal(
      &(lhs->free_space), &(rhs->free_space)))
  {
    return false;
  }
  // maximum_speed
  if (!std_msgs__msg__Float32__are_equal(
      &(lhs->maximum_speed), &(rhs->maximum_speed)))
  {
    return false;
  }
  return true;
}

bool
crp_msgs__msg__Scenario__copy(
  const crp_msgs__msg__Scenario * input,
  crp_msgs__msg__Scenario * output)
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
  // local_moving_objects
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__copy(
      &(input->local_moving_objects), &(output->local_moving_objects)))
  {
    return false;
  }
  // local_obstacles
  if (!autoware_perception_msgs__msg__PredictedObject__Sequence__copy(
      &(input->local_obstacles), &(output->local_obstacles)))
  {
    return false;
  }
  // paths
  if (!crp_msgs__msg__PathWithTrafficRules__Sequence__copy(
      &(input->paths), &(output->paths)))
  {
    return false;
  }
  // free_space
  if (!nav_msgs__msg__OccupancyGrid__copy(
      &(input->free_space), &(output->free_space)))
  {
    return false;
  }
  // maximum_speed
  if (!std_msgs__msg__Float32__copy(
      &(input->maximum_speed), &(output->maximum_speed)))
  {
    return false;
  }
  return true;
}

crp_msgs__msg__Scenario *
crp_msgs__msg__Scenario__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__Scenario * msg = (crp_msgs__msg__Scenario *)allocator.allocate(sizeof(crp_msgs__msg__Scenario), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(crp_msgs__msg__Scenario));
  bool success = crp_msgs__msg__Scenario__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
crp_msgs__msg__Scenario__destroy(crp_msgs__msg__Scenario * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    crp_msgs__msg__Scenario__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
crp_msgs__msg__Scenario__Sequence__init(crp_msgs__msg__Scenario__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__Scenario * data = NULL;

  if (size) {
    data = (crp_msgs__msg__Scenario *)allocator.zero_allocate(size, sizeof(crp_msgs__msg__Scenario), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = crp_msgs__msg__Scenario__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        crp_msgs__msg__Scenario__fini(&data[i - 1]);
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
crp_msgs__msg__Scenario__Sequence__fini(crp_msgs__msg__Scenario__Sequence * array)
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
      crp_msgs__msg__Scenario__fini(&array->data[i]);
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

crp_msgs__msg__Scenario__Sequence *
crp_msgs__msg__Scenario__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__Scenario__Sequence * array = (crp_msgs__msg__Scenario__Sequence *)allocator.allocate(sizeof(crp_msgs__msg__Scenario__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = crp_msgs__msg__Scenario__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
crp_msgs__msg__Scenario__Sequence__destroy(crp_msgs__msg__Scenario__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    crp_msgs__msg__Scenario__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
crp_msgs__msg__Scenario__Sequence__are_equal(const crp_msgs__msg__Scenario__Sequence * lhs, const crp_msgs__msg__Scenario__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!crp_msgs__msg__Scenario__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
crp_msgs__msg__Scenario__Sequence__copy(
  const crp_msgs__msg__Scenario__Sequence * input,
  crp_msgs__msg__Scenario__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(crp_msgs__msg__Scenario);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    crp_msgs__msg__Scenario * data =
      (crp_msgs__msg__Scenario *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!crp_msgs__msg__Scenario__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          crp_msgs__msg__Scenario__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!crp_msgs__msg__Scenario__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
