// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from crp_msgs:msg/Ego.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO__TRAITS_HPP_
#define CRP_MSGS__MSG__DETAIL__EGO__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "crp_msgs/msg/detail/ego__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__traits.hpp"
// Member 'twist'
#include "geometry_msgs/msg/detail/twist_with_covariance__traits.hpp"
// Member 'accel'
#include "geometry_msgs/msg/detail/accel_with_covariance__traits.hpp"

namespace crp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Ego & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: pose
  {
    out << "pose: ";
    to_flow_style_yaml(msg.pose, out);
    out << ", ";
  }

  // member: twist
  {
    out << "twist: ";
    to_flow_style_yaml(msg.twist, out);
    out << ", ";
  }

  // member: accel
  {
    out << "accel: ";
    to_flow_style_yaml(msg.accel, out);
    out << ", ";
  }

  // member: wheel_angle
  {
    out << "wheel_angle: ";
    rosidl_generator_traits::value_to_yaml(msg.wheel_angle, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Ego & msg,
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

  // member: pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pose:\n";
    to_block_style_yaml(msg.pose, out, indentation + 2);
  }

  // member: twist
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "twist:\n";
    to_block_style_yaml(msg.twist, out, indentation + 2);
  }

  // member: accel
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "accel:\n";
    to_block_style_yaml(msg.accel, out, indentation + 2);
  }

  // member: wheel_angle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "wheel_angle: ";
    rosidl_generator_traits::value_to_yaml(msg.wheel_angle, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Ego & msg, bool use_flow_style = false)
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
  const crp_msgs::msg::Ego & msg,
  std::ostream & out, size_t indentation = 0)
{
  crp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use crp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const crp_msgs::msg::Ego & msg)
{
  return crp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<crp_msgs::msg::Ego>()
{
  return "crp_msgs::msg::Ego";
}

template<>
inline const char * name<crp_msgs::msg::Ego>()
{
  return "crp_msgs/msg/Ego";
}

template<>
struct has_fixed_size<crp_msgs::msg::Ego>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::AccelWithCovariance>::value && has_fixed_size<geometry_msgs::msg::PoseWithCovariance>::value && has_fixed_size<geometry_msgs::msg::TwistWithCovariance>::value && has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<crp_msgs::msg::Ego>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::AccelWithCovariance>::value && has_bounded_size<geometry_msgs::msg::PoseWithCovariance>::value && has_bounded_size<geometry_msgs::msg::TwistWithCovariance>::value && has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<crp_msgs::msg::Ego>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CRP_MSGS__MSG__DETAIL__EGO__TRAITS_HPP_
