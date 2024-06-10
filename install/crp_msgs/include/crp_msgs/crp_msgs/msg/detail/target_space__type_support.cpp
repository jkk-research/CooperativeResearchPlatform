// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "crp_msgs/msg/detail/target_space__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace crp_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void TargetSpace_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) crp_msgs::msg::TargetSpace(_init);
}

void TargetSpace_fini_function(void * message_memory)
{
  auto typed_message = static_cast<crp_msgs::msg::TargetSpace *>(message_memory);
  typed_message->~TargetSpace();
}

size_t size_function__TargetSpace__relevant_objects(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TargetSpace__relevant_objects(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  return &member[index];
}

void * get_function__TargetSpace__relevant_objects(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  return &member[index];
}

void fetch_function__TargetSpace__relevant_objects(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const autoware_perception_msgs::msg::PredictedObject *>(
    get_const_function__TargetSpace__relevant_objects(untyped_member, index));
  auto & value = *reinterpret_cast<autoware_perception_msgs::msg::PredictedObject *>(untyped_value);
  value = item;
}

void assign_function__TargetSpace__relevant_objects(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<autoware_perception_msgs::msg::PredictedObject *>(
    get_function__TargetSpace__relevant_objects(untyped_member, index));
  const auto & value = *reinterpret_cast<const autoware_perception_msgs::msg::PredictedObject *>(untyped_value);
  item = value;
}

void resize_function__TargetSpace__relevant_objects(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TargetSpace__relevant_obstacles(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TargetSpace__relevant_obstacles(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  return &member[index];
}

void * get_function__TargetSpace__relevant_obstacles(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  return &member[index];
}

void fetch_function__TargetSpace__relevant_obstacles(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const autoware_perception_msgs::msg::PredictedObject *>(
    get_const_function__TargetSpace__relevant_obstacles(untyped_member, index));
  auto & value = *reinterpret_cast<autoware_perception_msgs::msg::PredictedObject *>(untyped_value);
  value = item;
}

void assign_function__TargetSpace__relevant_obstacles(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<autoware_perception_msgs::msg::PredictedObject *>(
    get_function__TargetSpace__relevant_obstacles(untyped_member, index));
  const auto & value = *reinterpret_cast<const autoware_perception_msgs::msg::PredictedObject *>(untyped_value);
  item = value;
}

void resize_function__TargetSpace__relevant_obstacles(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<autoware_perception_msgs::msg::PredictedObject> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember TargetSpace_message_member_array[6] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::TargetSpace, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "stopping_pose",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::PoseWithCovariance>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::TargetSpace, stopping_pose),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "free_space",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nav_msgs::msg::OccupancyGrid>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::TargetSpace, free_space),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "path",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<crp_msgs::msg::PathWithTrafficRules>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::TargetSpace, path),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "relevant_objects",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<autoware_perception_msgs::msg::PredictedObject>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::TargetSpace, relevant_objects),  // bytes offset in struct
    nullptr,  // default value
    size_function__TargetSpace__relevant_objects,  // size() function pointer
    get_const_function__TargetSpace__relevant_objects,  // get_const(index) function pointer
    get_function__TargetSpace__relevant_objects,  // get(index) function pointer
    fetch_function__TargetSpace__relevant_objects,  // fetch(index, &value) function pointer
    assign_function__TargetSpace__relevant_objects,  // assign(index, value) function pointer
    resize_function__TargetSpace__relevant_objects  // resize(index) function pointer
  },
  {
    "relevant_obstacles",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<autoware_perception_msgs::msg::PredictedObject>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::TargetSpace, relevant_obstacles),  // bytes offset in struct
    nullptr,  // default value
    size_function__TargetSpace__relevant_obstacles,  // size() function pointer
    get_const_function__TargetSpace__relevant_obstacles,  // get_const(index) function pointer
    get_function__TargetSpace__relevant_obstacles,  // get(index) function pointer
    fetch_function__TargetSpace__relevant_obstacles,  // fetch(index, &value) function pointer
    assign_function__TargetSpace__relevant_obstacles,  // assign(index, value) function pointer
    resize_function__TargetSpace__relevant_obstacles  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers TargetSpace_message_members = {
  "crp_msgs::msg",  // message namespace
  "TargetSpace",  // message name
  6,  // number of fields
  sizeof(crp_msgs::msg::TargetSpace),
  TargetSpace_message_member_array,  // message members
  TargetSpace_init_function,  // function to initialize message memory (memory has to be allocated)
  TargetSpace_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t TargetSpace_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &TargetSpace_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace crp_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<crp_msgs::msg::TargetSpace>()
{
  return &::crp_msgs::msg::rosidl_typesupport_introspection_cpp::TargetSpace_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, crp_msgs, msg, TargetSpace)() {
  return &::crp_msgs::msg::rosidl_typesupport_introspection_cpp::TargetSpace_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
