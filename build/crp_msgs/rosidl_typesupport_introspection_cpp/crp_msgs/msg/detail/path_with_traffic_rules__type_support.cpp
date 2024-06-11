// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from crp_msgs:msg/PathWithTrafficRules.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.hpp"
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

void PathWithTrafficRules_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) crp_msgs::msg::PathWithTrafficRules(_init);
}

void PathWithTrafficRules_fini_function(void * message_memory)
{
  auto typed_message = static_cast<crp_msgs::msg::PathWithTrafficRules *>(message_memory);
  typed_message->~PathWithTrafficRules();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember PathWithTrafficRules_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::PathWithTrafficRules, header),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<tier4_planning_msgs::msg::PathWithLaneId>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::PathWithTrafficRules, path),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "traffic_rules",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<crp_msgs::msg::TrafficRule>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(crp_msgs::msg::PathWithTrafficRules, traffic_rules),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers PathWithTrafficRules_message_members = {
  "crp_msgs::msg",  // message namespace
  "PathWithTrafficRules",  // message name
  3,  // number of fields
  sizeof(crp_msgs::msg::PathWithTrafficRules),
  PathWithTrafficRules_message_member_array,  // message members
  PathWithTrafficRules_init_function,  // function to initialize message memory (memory has to be allocated)
  PathWithTrafficRules_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t PathWithTrafficRules_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &PathWithTrafficRules_message_members,
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
get_message_type_support_handle<crp_msgs::msg::PathWithTrafficRules>()
{
  return &::crp_msgs::msg::rosidl_typesupport_introspection_cpp::PathWithTrafficRules_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, crp_msgs, msg, PathWithTrafficRules)() {
  return &::crp_msgs::msg::rosidl_typesupport_introspection_cpp::PathWithTrafficRules_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
