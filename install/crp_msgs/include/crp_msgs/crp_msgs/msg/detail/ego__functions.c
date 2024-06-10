// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from crp_msgs:msg/Ego.idl
// generated code does not contain a copyright notice
#include "crp_msgs/msg/detail/ego__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `pose`
#include "geometry_msgs/msg/detail/pose_with_covariance__functions.h"
// Member `twist`
#include "geometry_msgs/msg/detail/twist_with_covariance__functions.h"
// Member `accel`
#include "geometry_msgs/msg/detail/accel_with_covariance__functions.h"

bool
crp_msgs__msg__Ego__init(crp_msgs__msg__Ego * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    crp_msgs__msg__Ego__fini(msg);
    return false;
  }
  // pose
  if (!geometry_msgs__msg__PoseWithCovariance__init(&msg->pose)) {
    crp_msgs__msg__Ego__fini(msg);
    return false;
  }
  // twist
  if (!geometry_msgs__msg__TwistWithCovariance__init(&msg->twist)) {
    crp_msgs__msg__Ego__fini(msg);
    return false;
  }
  // accel
  if (!geometry_msgs__msg__AccelWithCovariance__init(&msg->accel)) {
    crp_msgs__msg__Ego__fini(msg);
    return false;
  }
  // wheel_angle
  return true;
}

void
crp_msgs__msg__Ego__fini(crp_msgs__msg__Ego * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // pose
  geometry_msgs__msg__PoseWithCovariance__fini(&msg->pose);
  // twist
  geometry_msgs__msg__TwistWithCovariance__fini(&msg->twist);
  // accel
  geometry_msgs__msg__AccelWithCovariance__fini(&msg->accel);
  // wheel_angle
}

bool
crp_msgs__msg__Ego__are_equal(const crp_msgs__msg__Ego * lhs, const crp_msgs__msg__Ego * rhs)
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
  // pose
  if (!geometry_msgs__msg__PoseWithCovariance__are_equal(
      &(lhs->pose), &(rhs->pose)))
  {
    return false;
  }
  // twist
  if (!geometry_msgs__msg__TwistWithCovariance__are_equal(
      &(lhs->twist), &(rhs->twist)))
  {
    return false;
  }
  // accel
  if (!geometry_msgs__msg__AccelWithCovariance__are_equal(
      &(lhs->accel), &(rhs->accel)))
  {
    return false;
  }
  // wheel_angle
  if (lhs->wheel_angle != rhs->wheel_angle) {
    return false;
  }
  return true;
}

bool
crp_msgs__msg__Ego__copy(
  const crp_msgs__msg__Ego * input,
  crp_msgs__msg__Ego * output)
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
  // pose
  if (!geometry_msgs__msg__PoseWithCovariance__copy(
      &(input->pose), &(output->pose)))
  {
    return false;
  }
  // twist
  if (!geometry_msgs__msg__TwistWithCovariance__copy(
      &(input->twist), &(output->twist)))
  {
    return false;
  }
  // accel
  if (!geometry_msgs__msg__AccelWithCovariance__copy(
      &(input->accel), &(output->accel)))
  {
    return false;
  }
  // wheel_angle
  output->wheel_angle = input->wheel_angle;
  return true;
}

crp_msgs__msg__Ego *
crp_msgs__msg__Ego__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__Ego * msg = (crp_msgs__msg__Ego *)allocator.allocate(sizeof(crp_msgs__msg__Ego), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(crp_msgs__msg__Ego));
  bool success = crp_msgs__msg__Ego__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
crp_msgs__msg__Ego__destroy(crp_msgs__msg__Ego * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    crp_msgs__msg__Ego__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
crp_msgs__msg__Ego__Sequence__init(crp_msgs__msg__Ego__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__Ego * data = NULL;

  if (size) {
    data = (crp_msgs__msg__Ego *)allocator.zero_allocate(size, sizeof(crp_msgs__msg__Ego), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = crp_msgs__msg__Ego__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        crp_msgs__msg__Ego__fini(&data[i - 1]);
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
crp_msgs__msg__Ego__Sequence__fini(crp_msgs__msg__Ego__Sequence * array)
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
      crp_msgs__msg__Ego__fini(&array->data[i]);
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

crp_msgs__msg__Ego__Sequence *
crp_msgs__msg__Ego__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  crp_msgs__msg__Ego__Sequence * array = (crp_msgs__msg__Ego__Sequence *)allocator.allocate(sizeof(crp_msgs__msg__Ego__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = crp_msgs__msg__Ego__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
crp_msgs__msg__Ego__Sequence__destroy(crp_msgs__msg__Ego__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    crp_msgs__msg__Ego__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
crp_msgs__msg__Ego__Sequence__are_equal(const crp_msgs__msg__Ego__Sequence * lhs, const crp_msgs__msg__Ego__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!crp_msgs__msg__Ego__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
crp_msgs__msg__Ego__Sequence__copy(
  const crp_msgs__msg__Ego__Sequence * input,
  crp_msgs__msg__Ego__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(crp_msgs__msg__Ego);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    crp_msgs__msg__Ego * data =
      (crp_msgs__msg__Ego *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!crp_msgs__msg__Ego__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          crp_msgs__msg__Ego__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!crp_msgs__msg__Ego__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
