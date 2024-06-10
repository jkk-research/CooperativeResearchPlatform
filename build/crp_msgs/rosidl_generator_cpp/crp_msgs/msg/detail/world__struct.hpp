// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/World.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__WORLD__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__WORLD__STRUCT_HPP_

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
# define DEPRECATED__crp_msgs__msg__World __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__World __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct World_
{
  using Type = World_<ContainerAllocator>;

  explicit World_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit World_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::World_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::World_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::World_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::World_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::World_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::World_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::World_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::World_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::World_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::World_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__World
    std::shared_ptr<crp_msgs::msg::World_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__World
    std::shared_ptr<crp_msgs::msg::World_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const World_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    return true;
  }
  bool operator!=(const World_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct World_

// alias to use template instance with default allocator
using World =
  crp_msgs::msg::World_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__WORLD__STRUCT_HPP_
