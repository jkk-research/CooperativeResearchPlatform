// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TARGET_SPACE__FUNCTIONS_H_
#define CRP_MSGS__MSG__DETAIL__TARGET_SPACE__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "crp_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "crp_msgs/msg/detail/target_space__struct.h"

/// Initialize msg/TargetSpace message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * crp_msgs__msg__TargetSpace
 * )) before or use
 * crp_msgs__msg__TargetSpace__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
bool
crp_msgs__msg__TargetSpace__init(crp_msgs__msg__TargetSpace * msg);

/// Finalize msg/TargetSpace message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
void
crp_msgs__msg__TargetSpace__fini(crp_msgs__msg__TargetSpace * msg);

/// Create msg/TargetSpace message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * crp_msgs__msg__TargetSpace__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
crp_msgs__msg__TargetSpace *
crp_msgs__msg__TargetSpace__create();

/// Destroy msg/TargetSpace message.
/**
 * It calls
 * crp_msgs__msg__TargetSpace__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
void
crp_msgs__msg__TargetSpace__destroy(crp_msgs__msg__TargetSpace * msg);

/// Check for msg/TargetSpace message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
bool
crp_msgs__msg__TargetSpace__are_equal(const crp_msgs__msg__TargetSpace * lhs, const crp_msgs__msg__TargetSpace * rhs);

/// Copy a msg/TargetSpace message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
bool
crp_msgs__msg__TargetSpace__copy(
  const crp_msgs__msg__TargetSpace * input,
  crp_msgs__msg__TargetSpace * output);

/// Initialize array of msg/TargetSpace messages.
/**
 * It allocates the memory for the number of elements and calls
 * crp_msgs__msg__TargetSpace__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
bool
crp_msgs__msg__TargetSpace__Sequence__init(crp_msgs__msg__TargetSpace__Sequence * array, size_t size);

/// Finalize array of msg/TargetSpace messages.
/**
 * It calls
 * crp_msgs__msg__TargetSpace__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
void
crp_msgs__msg__TargetSpace__Sequence__fini(crp_msgs__msg__TargetSpace__Sequence * array);

/// Create array of msg/TargetSpace messages.
/**
 * It allocates the memory for the array and calls
 * crp_msgs__msg__TargetSpace__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
crp_msgs__msg__TargetSpace__Sequence *
crp_msgs__msg__TargetSpace__Sequence__create(size_t size);

/// Destroy array of msg/TargetSpace messages.
/**
 * It calls
 * crp_msgs__msg__TargetSpace__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
void
crp_msgs__msg__TargetSpace__Sequence__destroy(crp_msgs__msg__TargetSpace__Sequence * array);

/// Check for msg/TargetSpace message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
bool
crp_msgs__msg__TargetSpace__Sequence__are_equal(const crp_msgs__msg__TargetSpace__Sequence * lhs, const crp_msgs__msg__TargetSpace__Sequence * rhs);

/// Copy an array of msg/TargetSpace messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_crp_msgs
bool
crp_msgs__msg__TargetSpace__Sequence__copy(
  const crp_msgs__msg__TargetSpace__Sequence * input,
  crp_msgs__msg__TargetSpace__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // CRP_MSGS__MSG__DETAIL__TARGET_SPACE__FUNCTIONS_H_
