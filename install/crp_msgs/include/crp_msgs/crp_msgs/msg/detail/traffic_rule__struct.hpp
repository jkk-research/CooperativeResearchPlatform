// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/TrafficRule.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__STRUCT_HPP_

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
// Member 'stop_pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__crp_msgs__msg__TrafficRule __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__TrafficRule __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TrafficRule_
{
  using Type = TrafficRule_<ContainerAllocator>;

  explicit TrafficRule_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    stop_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->lane_edge_type = 0;
    }
  }

  explicit TrafficRule_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    stop_pose(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->lane_edge_type = 0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _stop_pose_type =
    geometry_msgs::msg::PoseWithCovariance_<ContainerAllocator>;
  _stop_pose_type stop_pose;
  using _lane_edge_type_type =
    uint8_t;
  _lane_edge_type_type lane_edge_type;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__stop_pose(
    const geometry_msgs::msg::PoseWithCovariance_<ContainerAllocator> & _arg)
  {
    this->stop_pose = _arg;
    return *this;
  }
  Type & set__lane_edge_type(
    const uint8_t & _arg)
  {
    this->lane_edge_type = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::TrafficRule_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::TrafficRule_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::TrafficRule_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::TrafficRule_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__TrafficRule
    std::shared_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__TrafficRule
    std::shared_ptr<crp_msgs::msg::TrafficRule_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const TrafficRule_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->stop_pose != other.stop_pose) {
      return false;
    }
    if (this->lane_edge_type != other.lane_edge_type) {
      return false;
    }
    return true;
  }
  bool operator!=(const TrafficRule_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct TrafficRule_

// alias to use template instance with default allocator
using TrafficRule =
  crp_msgs::msg::TrafficRule_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__TRAFFIC_RULE__STRUCT_HPP_
