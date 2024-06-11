// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/PathWithTrafficRules.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'path'
#include "tier4_planning_msgs/msg/detail/path_with_lane_id__struct.hpp"
// Member 'traffic_rules'
#include "crp_msgs/msg/detail/traffic_rule__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__crp_msgs__msg__PathWithTrafficRules __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__PathWithTrafficRules __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct PathWithTrafficRules_
{
  using Type = PathWithTrafficRules_<ContainerAllocator>;

  explicit PathWithTrafficRules_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    path(_init),
    traffic_rules(_init)
  {
    (void)_init;
  }

  explicit PathWithTrafficRules_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    path(_alloc, _init),
    traffic_rules(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _path_type =
    tier4_planning_msgs::msg::PathWithLaneId_<ContainerAllocator>;
  _path_type path;
  using _traffic_rules_type =
    crp_msgs::msg::TrafficRule_<ContainerAllocator>;
  _traffic_rules_type traffic_rules;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__path(
    const tier4_planning_msgs::msg::PathWithLaneId_<ContainerAllocator> & _arg)
  {
    this->path = _arg;
    return *this;
  }
  Type & set__traffic_rules(
    const crp_msgs::msg::TrafficRule_<ContainerAllocator> & _arg)
  {
    this->traffic_rules = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__PathWithTrafficRules
    std::shared_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__PathWithTrafficRules
    std::shared_ptr<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PathWithTrafficRules_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->path != other.path) {
      return false;
    }
    if (this->traffic_rules != other.traffic_rules) {
      return false;
    }
    return true;
  }
  bool operator!=(const PathWithTrafficRules_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PathWithTrafficRules_

// alias to use template instance with default allocator
using PathWithTrafficRules =
  crp_msgs::msg::PathWithTrafficRules_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__PATH_WITH_TRAFFIC_RULES__STRUCT_HPP_
