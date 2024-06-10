// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/EgoStatus.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__EGO_STATUS__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__EGO_STATUS__STRUCT_HPP_

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

#ifndef _WIN32
# define DEPRECATED__crp_msgs__msg__EgoStatus __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__EgoStatus __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct EgoStatus_
{
  using Type = EgoStatus_<ContainerAllocator>;

  explicit EgoStatus_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->wheel_status = 0.0f;
    }
  }

  explicit EgoStatus_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->wheel_status = 0.0f;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _wheel_status_type =
    float;
  _wheel_status_type wheel_status;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__wheel_status(
    const float & _arg)
  {
    this->wheel_status = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::EgoStatus_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::EgoStatus_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::EgoStatus_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::EgoStatus_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__EgoStatus
    std::shared_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__EgoStatus
    std::shared_ptr<crp_msgs::msg::EgoStatus_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const EgoStatus_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->wheel_status != other.wheel_status) {
      return false;
    }
    return true;
  }
  bool operator!=(const EgoStatus_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct EgoStatus_

// alias to use template instance with default allocator
using EgoStatus =
  crp_msgs::msg::EgoStatus_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__EGO_STATUS__STRUCT_HPP_
