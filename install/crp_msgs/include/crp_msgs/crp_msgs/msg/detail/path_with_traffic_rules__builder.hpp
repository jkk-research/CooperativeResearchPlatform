// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/PathWithTrafficRules.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_PathWithTrafficRules_traffic_rules
{
public:
  explicit Init_PathWithTrafficRules_traffic_rules(::crp_msgs::msg::PathWithTrafficRules & msg)
  : msg_(msg)
  {}
  ::crp_msgs::msg::PathWithTrafficRules traffic_rules(::crp_msgs::msg::PathWithTrafficRules::_traffic_rules_type arg)
  {
    msg_.traffic_rules = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::PathWithTrafficRules msg_;
};

class Init_PathWithTrafficRules_path
{
public:
  explicit Init_PathWithTrafficRules_path(::crp_msgs::msg::PathWithTrafficRules & msg)
  : msg_(msg)
  {}
  Init_PathWithTrafficRules_traffic_rules path(::crp_msgs::msg::PathWithTrafficRules::_path_type arg)
  {
    msg_.path = std::move(arg);
    return Init_PathWithTrafficRules_traffic_rules(msg_);
  }

private:
  ::crp_msgs::msg::PathWithTrafficRules msg_;
};

class Init_PathWithTrafficRules_header
{
public:
  Init_PathWithTrafficRules_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PathWithTrafficRules_path header(::crp_msgs::msg::PathWithTrafficRules::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_PathWithTrafficRules_path(msg_);
  }

private:
  ::crp_msgs::msg::PathWithTrafficRules msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::PathWithTrafficRules>()
{
  return crp_msgs::msg::builder::Init_PathWithTrafficRules_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__BUILDER_HPP_
