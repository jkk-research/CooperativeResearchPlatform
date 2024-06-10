// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from crp_msgs:msg/TrafficRule.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__TRAITS_HPP_
#define CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "crp_msgs/msg/detail/traffic_rule__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'stop_pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__traits.hpp"

namespace crp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const TrafficRule & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: stop_pose
  {
    out << "stop_pose: ";
    to_flow_style_yaml(msg.stop_pose, out);
    out << ", ";
  }

  // member: lane_edge_type
  {
    out << "lane_edge_type: ";
    rosidl_generator_traits::value_to_yaml(msg.lane_edge_type, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const TrafficRule & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: stop_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stop_pose:\n";
    to_block_style_yaml(msg.stop_pose, out, indentation + 2);
  }

  // member: lane_edge_type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "lane_edge_type: ";
    rosidl_generator_traits::value_to_yaml(msg.lane_edge_type, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const TrafficRule & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace crp_msgs

namespace rosidl_generator_traits
{

[[deprecated("use crp_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const crp_msgs::msg::TrafficRule & msg,
  std::ostream & out, size_t indentation = 0)
{
  crp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use crp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const crp_msgs::msg::TrafficRule & msg)
{
  return crp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<crp_msgs::msg::TrafficRule>()
{
  return "crp_msgs::msg::TrafficRule";
}

template<>
inline const char * name<crp_msgs::msg::TrafficRule>()
{
  return "crp_msgs/msg/TrafficRule";
}

template<>
struct has_fixed_size<crp_msgs::msg::TrafficRule>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::PoseWithCovariance>::value && has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<crp_msgs::msg::TrafficRule>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::PoseWithCovariance>::value && has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<crp_msgs::msg::TrafficRule>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__TRAITS_HPP_
