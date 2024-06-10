// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/Ego.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__EGO__STRUCT_HPP_

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
// Member 'pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__struct.hpp"
// Member 'twist'
#include "geometry_msgs/msg/detail/twist_with_covariance__struct.hpp"
// Member 'accel'
#include "geometry_msgs/msg/detail/accel_with_covariance__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__crp_msgs__msg__Ego __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__Ego __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Ego_
{
  using Type = Ego_<ContainerAllocator>;

  explicit Ego_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    pose(_init),
    twist(_init),
    accel(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->wheel_angle = 0.0f;
    }
  }

  explicit Ego_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    pose(_alloc, _init),
    twist(_alloc, _init),
    accel(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->wheel_angle = 0.0f;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _pose_type =
    geometry_msgs::msg::PoseWithCovariance_<ContainerAllocator>;
  _pose_type pose;
  using _twist_type =
    geometry_msgs::msg::TwistWithCovariance_<ContainerAllocator>;
  _twist_type twist;
  using _accel_type =
    geometry_msgs::msg::AccelWithCovariance_<ContainerAllocator>;
  _accel_type accel;
  using _wheel_angle_type =
    float;
  _wheel_angle_type wheel_angle;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__pose(
    const geometry_msgs::msg::PoseWithCovariance_<ContainerAllocator> & _arg)
  {
    this->pose = _arg;
    return *this;
  }
  Type & set__twist(
    const geometry_msgs::msg::TwistWithCovariance_<ContainerAllocator> & _arg)
  {
    this->twist = _arg;
    return *this;
  }
  Type & set__accel(
    const geometry_msgs::msg::AccelWithCovariance_<ContainerAllocator> & _arg)
  {
    this->accel = _arg;
    return *this;
  }
  Type & set__wheel_angle(
    const float & _arg)
  {
    this->wheel_angle = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::Ego_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::Ego_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::Ego_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::Ego_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::Ego_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::Ego_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::Ego_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::Ego_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::Ego_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::Ego_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__Ego
    std::shared_ptr<crp_msgs::msg::Ego_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__Ego
    std::shared_ptr<crp_msgs::msg::Ego_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Ego_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->pose != other.pose) {
      return false;
    }
    if (this->twist != other.twist) {
      return false;
    }
    if (this->accel != other.accel) {
      return false;
    }
    if (this->wheel_angle != other.wheel_angle) {
      return false;
    }
    return true;
  }
  bool operator!=(const Ego_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Ego_

// alias to use template instance with default allocator
using Ego =
  crp_msgs::msg::Ego_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__EGO__STRUCT_HPP_
