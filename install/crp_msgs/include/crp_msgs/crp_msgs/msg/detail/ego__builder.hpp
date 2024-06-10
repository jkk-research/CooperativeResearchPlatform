// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/Ego.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__EGO__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/ego__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_Ego_wheel_angle
{
public:
  explicit Init_Ego_wheel_angle(::crp_msgs::msg::Ego & msg)
  : msg_(msg)
  {}
  ::crp_msgs::msg::Ego wheel_angle(::crp_msgs::msg::Ego::_wheel_angle_type arg)
  {
    msg_.wheel_angle = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::Ego msg_;
};

class Init_Ego_accel
{
public:
  explicit Init_Ego_accel(::crp_msgs::msg::Ego & msg)
  : msg_(msg)
  {}
  Init_Ego_wheel_angle accel(::crp_msgs::msg::Ego::_accel_type arg)
  {
    msg_.accel = std::move(arg);
    return Init_Ego_wheel_angle(msg_);
  }

private:
  ::crp_msgs::msg::Ego msg_;
};

class Init_Ego_twist
{
public:
  explicit Init_Ego_twist(::crp_msgs::msg::Ego & msg)
  : msg_(msg)
  {}
  Init_Ego_accel twist(::crp_msgs::msg::Ego::_twist_type arg)
  {
    msg_.twist = std::move(arg);
    return Init_Ego_accel(msg_);
  }

private:
  ::crp_msgs::msg::Ego msg_;
};

class Init_Ego_pose
{
public:
  explicit Init_Ego_pose(::crp_msgs::msg::Ego & msg)
  : msg_(msg)
  {}
  Init_Ego_twist pose(::crp_msgs::msg::Ego::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return Init_Ego_twist(msg_);
  }

private:
  ::crp_msgs::msg::Ego msg_;
};

class Init_Ego_header
{
public:
  Init_Ego_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Ego_pose header(::crp_msgs::msg::Ego::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Ego_pose(msg_);
  }

private:
  ::crp_msgs::msg::Ego msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::Ego>()
{
  return crp_msgs::msg::builder::Init_Ego_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__EGO__BUILDER_HPP_
