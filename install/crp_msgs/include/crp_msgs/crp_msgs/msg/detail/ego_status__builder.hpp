// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/EgoStatus.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO_STATUS__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__EGO_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/ego_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_EgoStatus_wheel_status
{
public:
  explicit Init_EgoStatus_wheel_status(::crp_msgs::msg::EgoStatus & msg)
  : msg_(msg)
  {}
  ::crp_msgs::msg::EgoStatus wheel_status(::crp_msgs::msg::EgoStatus::_wheel_status_type arg)
  {
    msg_.wheel_status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::EgoStatus msg_;
};

class Init_EgoStatus_header
{
public:
  Init_EgoStatus_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_EgoStatus_wheel_status header(::crp_msgs::msg::EgoStatus::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_EgoStatus_wheel_status(msg_);
  }

private:
  ::crp_msgs::msg::EgoStatus msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::EgoStatus>()
{
  return crp_msgs::msg::builder::Init_EgoStatus_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__EGO_STATUS__BUILDER_HPP_
