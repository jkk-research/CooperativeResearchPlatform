// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from crp_msgs:msg/PathWithTrafficRules.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__TRAITS_HPP_
#define CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'path'
#include "tier4_planning_msgs/msg/detail/path_with_lane_id__traits.hpp"
// Member 'traffic_rules'
#include "crp_msgs/msg/detail/traffic_rule__traits.hpp"

namespace crp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const PathWithTrafficRules & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: path
  {
    out << "path: ";
    to_flow_style_yaml(msg.path, out);
    out << ", ";
  }

  // member: traffic_rules
  {
    if (msg.traffic_rules.size() == 0) {
      out << "traffic_rules: []";
    } else {
      out << "traffic_rules: [";
      size_t pending_items = msg.traffic_rules.size();
      for (auto item : msg.traffic_rules) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PathWithTrafficRules & msg,
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

  // member: path
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "path:\n";
    to_block_style_yaml(msg.path, out, indentation + 2);
  }

  // member: traffic_rules
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.traffic_rules.size() == 0) {
      out << "traffic_rules: []\n";
    } else {
      out << "traffic_rules:\n";
      for (auto item : msg.traffic_rules) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PathWithTrafficRules & msg, bool use_flow_style = false)
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
  const crp_msgs::msg::PathWithTrafficRules & msg,
  std::ostream & out, size_t indentation = 0)
{
  crp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use crp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const crp_msgs::msg::PathWithTrafficRules & msg)
{
  return crp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<crp_msgs::msg::PathWithTrafficRules>()
{
  return "crp_msgs::msg::PathWithTrafficRules";
}

template<>
inline const char * name<crp_msgs::msg::PathWithTrafficRules>()
{
  return "crp_msgs/msg/PathWithTrafficRules";
}

template<>
struct has_fixed_size<crp_msgs::msg::PathWithTrafficRules>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<crp_msgs::msg::PathWithTrafficRules>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<crp_msgs::msg::PathWithTrafficRules>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__TRAITS_HPP_
