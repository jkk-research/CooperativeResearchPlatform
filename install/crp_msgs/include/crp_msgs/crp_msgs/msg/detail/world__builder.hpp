// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/World.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__WORLD__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__WORLD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/world__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_World_header
{
public:
  Init_World_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::crp_msgs::msg::World header(::crp_msgs::msg::World::_header_type arg)
  {
    msg_.header = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::World msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::World>()
{
  return crp_msgs::msg::builder::Init_World_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__WORLD__BUILDER_HPP_
