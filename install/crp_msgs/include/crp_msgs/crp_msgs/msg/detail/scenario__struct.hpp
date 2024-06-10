// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice

#ifndef CRP_MSGS__MSG__DETAIL__SCENARIO__STRUCT_HPP_
#define CRP_MSGS__MSG__DETAIL__SCENARIO__STRUCT_HPP_

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
// Member 'local_moving_objects'
// Member 'local_obstacles'
#include "autoware_perception_msgs/msg/detail/predicted_object__struct.hpp"
// Member 'paths'
#include "crp_msgs/msg/detail/path_with_traffic_rules__struct.hpp"
// Member 'free_space'
#include "nav_msgs/msg/detail/occupancy_grid__struct.hpp"
// Member 'maximum_speed'
#include "std_msgs/msg/detail/float32__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__crp_msgs__msg__Scenario __attribute__((deprecated))
#else
# define DEPRECATED__crp_msgs__msg__Scenario __declspec(deprecated)
#endif

namespace crp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Scenario_
{
  using Type = Scenario_<ContainerAllocator>;

  explicit Scenario_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    free_space(_init),
    maximum_speed(_init)
  {
    (void)_init;
  }

  explicit Scenario_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    free_space(_alloc, _init),
    maximum_speed(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _local_moving_objects_type =
    std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>>;
  _local_moving_objects_type local_moving_objects;
  using _local_obstacles_type =
    std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>>;
  _local_obstacles_type local_obstacles;
  using _paths_type =
    std::vector<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>>;
  _paths_type paths;
  using _free_space_type =
    nav_msgs::msg::OccupancyGrid_<ContainerAllocator>;
  _free_space_type free_space;
  using _maximum_speed_type =
    std_msgs::msg::Float32_<ContainerAllocator>;
  _maximum_speed_type maximum_speed;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__local_moving_objects(
    const std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>> & _arg)
  {
    this->local_moving_objects = _arg;
    return *this;
  }
  Type & set__local_obstacles(
    const std::vector<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<autoware_perception_msgs::msg::PredictedObject_<ContainerAllocator>>> & _arg)
  {
    this->local_obstacles = _arg;
    return *this;
  }
  Type & set__paths(
    const std::vector<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<crp_msgs::msg::PathWithTrafficRules_<ContainerAllocator>>> & _arg)
  {
    this->paths = _arg;
    return *this;
  }
  Type & set__free_space(
    const nav_msgs::msg::OccupancyGrid_<ContainerAllocator> & _arg)
  {
    this->free_space = _arg;
    return *this;
  }
  Type & set__maximum_speed(
    const std_msgs::msg::Float32_<ContainerAllocator> & _arg)
  {
    this->maximum_speed = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    crp_msgs::msg::Scenario_<ContainerAllocator> *;
  using ConstRawPtr =
    const crp_msgs::msg::Scenario_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<crp_msgs::msg::Scenario_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<crp_msgs::msg::Scenario_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::Scenario_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::Scenario_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      crp_msgs::msg::Scenario_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<crp_msgs::msg::Scenario_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<crp_msgs::msg::Scenario_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<crp_msgs::msg::Scenario_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__crp_msgs__msg__Scenario
    std::shared_ptr<crp_msgs::msg::Scenario_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__crp_msgs__msg__Scenario
    std::shared_ptr<crp_msgs::msg::Scenario_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Scenario_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->local_moving_objects != other.local_moving_objects) {
      return false;
    }
    if (this->local_obstacles != other.local_obstacles) {
      return false;
    }
    if (this->paths != other.paths) {
      return false;
    }
    if (this->free_space != other.free_space) {
      return false;
    }
    if (this->maximum_speed != other.maximum_speed) {
      return false;
    }
    return true;
  }
  bool operator!=(const Scenario_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Scenario_

// alias to use template instance with default allocator
using Scenario =
  crp_msgs::msg::Scenario_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace crp_msgs

#endif  // CRP_MSGS__MSG__DETAIL__SCENARIO__STRUCT_HPP_
