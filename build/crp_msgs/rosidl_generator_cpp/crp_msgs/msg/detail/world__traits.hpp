// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from crp_msgs:msg/World.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__WORLD__TRAITS_HPP_
#define CRP_MSGS__MSG__DETAIL__WORLD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "crp_msgs/msg/detail/world__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace crp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const World & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const World & msg,
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
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const World & msg, bool use_flow_style = false)
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
  const crp_msgs::msg::World & msg,
  std::ostream & out, size_t indentation = 0)
{
  crp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use crp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const crp_msgs::msg::World & msg)
{
  return crp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<crp_msgs::msg::World>()
{
  return "crp_msgs::msg::World";
}

template<>
inline const char * name<crp_msgs::msg::World>()
{
  return "crp_msgs/msg/World";
}

template<>
struct has_fixed_size<crp_msgs::msg::World>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<crp_msgs::msg::World>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<crp_msgs::msg::World>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CRP_MSGS__MSG__DETAIL__WORLD__TRAITS_HPP_
