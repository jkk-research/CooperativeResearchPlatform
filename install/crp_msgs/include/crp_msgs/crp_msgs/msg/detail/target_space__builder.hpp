// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TARGET_SPACE__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__TARGET_SPACE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/target_space__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_TargetSpace_relevant_obstacles
{
public:
  explicit Init_TargetSpace_relevant_obstacles(::crp_msgs::msg::TargetSpace & msg)
  : msg_(msg)
  {}
  ::crp_msgs::msg::TargetSpace relevant_obstacles(::crp_msgs::msg::TargetSpace::_relevant_obstacles_type arg)
  {
    msg_.relevant_obstacles = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::TargetSpace msg_;
};

class Init_TargetSpace_relevant_objects
{
public:
  explicit Init_TargetSpace_relevant_objects(::crp_msgs::msg::TargetSpace & msg)
  : msg_(msg)
  {}
  Init_TargetSpace_relevant_obstacles relevant_objects(::crp_msgs::msg::TargetSpace::_relevant_objects_type arg)
  {
    msg_.relevant_objects = std::move(arg);
    return Init_TargetSpace_relevant_obstacles(msg_);
  }

private:
  ::crp_msgs::msg::TargetSpace msg_;
};

class Init_TargetSpace_path
{
public:
  explicit Init_TargetSpace_path(::crp_msgs::msg::TargetSpace & msg)
  : msg_(msg)
  {}
  Init_TargetSpace_relevant_objects path(::crp_msgs::msg::TargetSpace::_path_type arg)
  {
    msg_.path = std::move(arg);
    return Init_TargetSpace_relevant_objects(msg_);
  }

private:
  ::crp_msgs::msg::TargetSpace msg_;
};

class Init_TargetSpace_free_space
{
public:
  explicit Init_TargetSpace_free_space(::crp_msgs::msg::TargetSpace & msg)
  : msg_(msg)
  {}
  Init_TargetSpace_path free_space(::crp_msgs::msg::TargetSpace::_free_space_type arg)
  {
    msg_.free_space = std::move(arg);
    return Init_TargetSpace_path(msg_);
  }

private:
  ::crp_msgs::msg::TargetSpace msg_;
};

class Init_TargetSpace_target_pose
{
public:
  explicit Init_TargetSpace_target_pose(::crp_msgs::msg::TargetSpace & msg)
  : msg_(msg)
  {}
  Init_TargetSpace_free_space target_pose(::crp_msgs::msg::TargetSpace::_target_pose_type arg)
  {
    msg_.target_pose = std::move(arg);
    return Init_TargetSpace_free_space(msg_);
  }

private:
  ::crp_msgs::msg::TargetSpace msg_;
};

class Init_TargetSpace_header
{
public:
  Init_TargetSpace_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TargetSpace_target_pose header(::crp_msgs::msg::TargetSpace::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_TargetSpace_target_pose(msg_);
  }

private:
  ::crp_msgs::msg::TargetSpace msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::TargetSpace>()
{
  return crp_msgs::msg::builder::Init_TargetSpace_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__TARGET_SPACE__BUILDER_HPP_
