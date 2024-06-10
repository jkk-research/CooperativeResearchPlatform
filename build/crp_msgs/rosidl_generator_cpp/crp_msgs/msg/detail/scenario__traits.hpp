// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__SCENARIO__TRAITS_HPP_
#define CRP_MSGS__MSG__DETAIL__SCENARIO__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "crp_msgs/msg/detail/scenario__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'local_moving_objects'
// Member 'local_obstacles'
#include "autoware_perception_msgs/msg/detail/predicted_object__traits.hpp"
// Member 'paths'
#include "crp_msgs/msg/detail/path_with_traffic_rules__traits.hpp"
// Member 'free_space'
#include "nav_msgs/msg/detail/occupancy_grid__traits.hpp"
// Member 'maximum_speed'
#include "std_msgs/msg/detail/float32__traits.hpp"

namespace crp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Scenario & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: local_moving_objects
  {
    if (msg.local_moving_objects.size() == 0) {
      out << "local_moving_objects: []";
    } else {
      out << "local_moving_objects: [";
      size_t pending_items = msg.local_moving_objects.size();
      for (auto item : msg.local_moving_objects) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: local_obstacles
  {
    if (msg.local_obstacles.size() == 0) {
      out << "local_obstacles: []";
    } else {
      out << "local_obstacles: [";
      size_t pending_items = msg.local_obstacles.size();
      for (auto item : msg.local_obstacles) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: paths
  {
    if (msg.paths.size() == 0) {
      out << "paths: []";
    } else {
      out << "paths: [";
      size_t pending_items = msg.paths.size();
      for (auto item : msg.paths) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: free_space
  {
    out << "free_space: ";
    to_flow_style_yaml(msg.free_space, out);
    out << ", ";
  }

  // member: maximum_speed
  {
    out << "maximum_speed: ";
    to_flow_style_yaml(msg.maximum_speed, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Scenario & msg,
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

  // member: local_moving_objects
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.local_moving_objects.size() == 0) {
      out << "local_moving_objects: []\n";
    } else {
      out << "local_moving_objects:\n";
      for (auto item : msg.local_moving_objects) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: local_obstacles
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.local_obstacles.size() == 0) {
      out << "local_obstacles: []\n";
    } else {
      out << "local_obstacles:\n";
      for (auto item : msg.local_obstacles) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: paths
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.paths.size() == 0) {
      out << "paths: []\n";
    } else {
      out << "paths:\n";
      for (auto item : msg.paths) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: free_space
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "free_space:\n";
    to_block_style_yaml(msg.free_space, out, indentation + 2);
  }

  // member: maximum_speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "maximum_speed:\n";
    to_block_style_yaml(msg.maximum_speed, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Scenario & msg, bool use_flow_style = false)
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
  const crp_msgs::msg::Scenario & msg,
  std::ostream & out, size_t indentation = 0)
{
  crp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use crp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const crp_msgs::msg::Scenario & msg)
{
  return crp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<crp_msgs::msg::Scenario>()
{
  return "crp_msgs::msg::Scenario";
}

template<>
inline const char * name<crp_msgs::msg::Scenario>()
{
  return "crp_msgs/msg/Scenario";
}

template<>
struct has_fixed_size<crp_msgs::msg::Scenario>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<crp_msgs::msg::Scenario>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<crp_msgs::msg::Scenario>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CRP_MSGS__MSG__DETAIL__SCENARIO__TRAITS_HPP_
