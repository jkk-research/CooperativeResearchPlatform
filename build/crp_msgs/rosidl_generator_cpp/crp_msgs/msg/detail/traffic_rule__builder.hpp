// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from crp_msgs:msg/TrafficRule.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__BUILDER_HPP_
#define CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "crp_msgs/msg/detail/traffic_rule__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace crp_msgs
{

namespace msg
{

namespace builder
{

class Init_TrafficRule_lane_edge_type
{
public:
  explicit Init_TrafficRule_lane_edge_type(::crp_msgs::msg::TrafficRule & msg)
  : msg_(msg)
  {}
  ::crp_msgs::msg::TrafficRule lane_edge_type(::crp_msgs::msg::TrafficRule::_lane_edge_type_type arg)
  {
    msg_.lane_edge_type = std::move(arg);
    return std::move(msg_);
  }

private:
  ::crp_msgs::msg::TrafficRule msg_;
};

class Init_TrafficRule_stop_pose
{
public:
  explicit Init_TrafficRule_stop_pose(::crp_msgs::msg::TrafficRule & msg)
  : msg_(msg)
  {}
  Init_TrafficRule_lane_edge_type stop_pose(::crp_msgs::msg::TrafficRule::_stop_pose_type arg)
  {
    msg_.stop_pose = std::move(arg);
    return Init_TrafficRule_lane_edge_type(msg_);
  }

private:
  ::crp_msgs::msg::TrafficRule msg_;
};

class Init_TrafficRule_header
{
public:
  Init_TrafficRule_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrafficRule_stop_pose header(::crp_msgs::msg::TrafficRule::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_TrafficRule_stop_pose(msg_);
  }

private:
  ::crp_msgs::msg::TrafficRule msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::crp_msgs::msg::TrafficRule>()
{
  return crp_msgs::msg::builder::Init_TrafficRule_header();
}

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__BUILDER_HPP_
