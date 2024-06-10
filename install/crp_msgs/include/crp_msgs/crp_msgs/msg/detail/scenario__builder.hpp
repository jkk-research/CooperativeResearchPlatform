// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__SCENARIO__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__SCENARIO__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/scenario__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_Scenario_maximum_speed
{
public:
  explicit Init_Scenario_maximum_speed(::crp_msgs::msg::Scenario & msg)
  : msg_(msg)
  {}
  ::crp_msgs::msg::Scenario maximum_speed(::crp_msgs::msg::Scenario::_maximum_speed_type arg)
  {
    msg_.maximum_speed = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::Scenario msg_;
};

class Init_Scenario_free_space
{
public:
  explicit Init_Scenario_free_space(::crp_msgs::msg::Scenario & msg)
  : msg_(msg)
  {}
  Init_Scenario_maximum_speed free_space(::crp_msgs::msg::Scenario::_free_space_type arg)
  {
    msg_.free_space = std::move(arg);
    return Init_Scenario_maximum_speed(msg_);
  }

private:
  ::crp_msgs::msg::Scenario msg_;
};

class Init_Scenario_paths
{
public:
  explicit Init_Scenario_paths(::crp_msgs::msg::Scenario & msg)
  : msg_(msg)
  {}
  Init_Scenario_free_space paths(::crp_msgs::msg::Scenario::_paths_type arg)
  {
    msg_.paths = std::move(arg);
    return Init_Scenario_free_space(msg_);
  }

private:
  ::crp_msgs::msg::Scenario msg_;
};

class Init_Scenario_local_obstacles
{
public:
  explicit Init_Scenario_local_obstacles(::crp_msgs::msg::Scenario & msg)
  : msg_(msg)
  {}
  Init_Scenario_paths local_obstacles(::crp_msgs::msg::Scenario::_local_obstacles_type arg)
  {
    msg_.local_obstacles = std::move(arg);
    return Init_Scenario_paths(msg_);
  }

private:
  ::crp_msgs::msg::Scenario msg_;
};

class Init_Scenario_local_moving_objects
{
public:
  explicit Init_Scenario_local_moving_objects(::crp_msgs::msg::Scenario & msg)
  : msg_(msg)
  {}
  Init_Scenario_local_obstacles local_moving_objects(::crp_msgs::msg::Scenario::_local_moving_objects_type arg)
  {
    msg_.local_moving_objects = std::move(arg);
    return Init_Scenario_local_obstacles(msg_);
  }

private:
  ::crp_msgs::msg::Scenario msg_;
};

class Init_Scenario_header
{
public:
  Init_Scenario_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Scenario_local_moving_objects header(::crp_msgs::msg::Scenario::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Scenario_local_moving_objects(msg_);
  }

private:
  ::crp_msgs::msg::Scenario msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::Scenario>()
{
  return crp_msgs::msg::builder::Init_Scenario_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__SCENARIO__BUILDER_HPP_
