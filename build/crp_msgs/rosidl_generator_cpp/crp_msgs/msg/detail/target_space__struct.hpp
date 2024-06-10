// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/TargetSpace.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__TARGET_SPACE__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__TARGET_SPACE__STRUCT_HPP_

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
// Member 'stopping_pose'
#include "geometry_msgs/msg/detail/pose_with_covariance__struct.hpp"
// Member 'free_space'
#include "nav_msgs/msg/detail/occupancy_grid__struct.hpp"
// Member 'path'
#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.hpp"
// Member 'relevant_objects'
// Member 'relevant_obstacles'
#include "autoware_perception_msgs/msg/detail/predicted_object__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__crp_msgs__msg__TargetSpace __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__TargetSpace __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TargetSpace_
{
  using Type = TargetSpace_<ContainerAllocator>;

  explicit TargetSpace_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    stopping_pose(_init),
    free_space(_init),
    path(_init)
  {
    (void)_init;
  }

  explicit TargetSpace_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    stopping_pose(_alloc, _init),
    free_space(_alloc, _init),
    path(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _stopping_pose_type =
    geometry_msgs::msg::PoseWithCovariance_<ContainerAllocator>;
  _stopping_pose_type stopping_pose;
  using _free_space_type =
    nav_msgs::msg::OccupancyGrid_<ContainerAllocator>;
  _free_space_type free_space;
  using _path_type =
    crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>;
  _path_type path;
  using _relevant_objects_type =
    std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>>;
  _relevant_objects_type relevant_objects;
  using _relevant_obstacles_type =
    std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>>;
  _relevant_obstacles_type relevant_obstacles;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__stopping_pose(
    const geometry_msgs::msg::PoseWithCovariance_<ContainerAllocator> & _arg)
  {
    this->stopping_pose = _arg;
    return *this;
  }
  Type & set__free_space(
    const nav_msgs::msg::OccupancyGrid_<ContainerAllocator> & _arg)
  {
    this->free_space = _arg;
    return *this;
  }
  Type & set__path(
    const crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator> & _arg)
  {
    this->path = _arg;
    return *this;
  }
  Type & set__relevant_objects(
    const std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>> & _arg)
  {
    this->relevant_objects = _arg;
    return *this;
  }
  Type & set__relevant_obstacles(
    const std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>> & _arg)
  {
    this->relevant_obstacles = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::TargetSpace_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::TargetSpace_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::TargetSpace_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::TargetSpace_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__TargetSpace
    std::shared_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__TargetSpace
    std::shared_ptr<crp_msgs::msg::TargetSpace_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const TargetSpace_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->stopping_pose != other.stopping_pose) {
      return false;
    }
    if (this->free_space != other.free_space) {
      return false;
    }
    if (this->path != other.path) {
      return false;
    }
    if (this->relevant_objects != other.relevant_objects) {
      return false;
    }
    if (this->relevant_obstacles != other.relevant_obstacles) {
      return false;
    }
    return true;
  }
  bool operator!=(const TargetSpace_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct TargetSpace_

// alias to use template instance with default allocator
using TargetSpace =
  crp_msgs::msg::TargetSpace_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__TARGET_SPACE__STRUCT_HPP_
