// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TARGET_SPACE__TRAITS_HPP_
#define CRP_MSGS__MSG__DETAIL__TARGET_SPACE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "crp_msgs/msg/detail/target_space__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'stopping_pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__traits.hpp"
// Member 'free_space'
#include "nav_msgs/msg/detail/occupancy_grid__traits.hpp"
// Member 'path'
#include "crp_msgs/msg/detail/path_with_traffic_rules__traits.hpp"
// Member 'relevant_objects'
// Member 'relevant_obstacles'
#include "autoware_perception_msgs/msg/detail/predicted_object__traits.hpp"

namespace crp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const TargetSpace & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: stopping_pose
  {
    out << "stopping_pose: ";
    to_flow_style_yaml(msg.stopping_pose, out);
    out << ", ";
  }

  // member: free_space
  {
    out << "free_space: ";
    to_flow_style_yaml(msg.free_space, out);
    out << ", ";
  }

  // member: path
  {
    out << "path: ";
    to_flow_style_yaml(msg.path, out);
    out << ", ";
  }

  // member: relevant_objects
  {
    if (msg.relevant_objects.size() == 0) {
      out << "relevant_objects: []";
    } else {
      out << "relevant_objects: [";
      size_t pending_items = msg.relevant_objects.size();
      for (auto item : msg.relevant_objects) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: relevant_obstacles
  {
    if (msg.relevant_obstacles.size() == 0) {
      out << "relevant_obstacles: []";
    } else {
      out << "relevant_obstacles: [";
      size_t pending_items = msg.relevant_obstacles.size();
      for (auto item : msg.relevant_obstacles) {
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
  const TargetSpace & msg,
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

  // member: stopping_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stopping_pose:\n";
    to_block_style_yaml(msg.stopping_pose, out, indentation + 2);
  }

  // member: free_space
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "free_space:\n";
    to_block_style_yaml(msg.free_space, out, indentation + 2);
  }

  // member: path
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "path:\n";
    to_block_style_yaml(msg.path, out, indentation + 2);
  }

  // member: relevant_objects
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.relevant_objects.size() == 0) {
      out << "relevant_objects: []\n";
    } else {
      out << "relevant_objects:\n";
      for (auto item : msg.relevant_objects) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: relevant_obstacles
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.relevant_obstacles.size() == 0) {
      out << "relevant_obstacles: []\n";
    } else {
      out << "relevant_obstacles:\n";
      for (auto item : msg.relevant_obstacles) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const TargetSpace & msg, bool use_flow_style = false)
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
  const crp_msgs::msg::TargetSpace & msg,
  std::ostream & out, size_t indentation = 0)
{
  crp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use crp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const crp_msgs::msg::TargetSpace & msg)
{
  return crp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<crp_msgs::msg::TargetSpace>()
{
  return "crp_msgs::msg::TargetSpace";
}

template<>
inline const char * name<crp_msgs::msg::TargetSpace>()
{
  return "crp_msgs/msg/TargetSpace";
}

template<>
struct has_fixed_size<crp_msgs::msg::TargetSpace>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<crp_msgs::msg::TargetSpace>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<crp_msgs::msg::TargetSpace>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CRP_MSGS__MSG__DETAIL__TARGET_SPACE__TRAITS_HPP_
