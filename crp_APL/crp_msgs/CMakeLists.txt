cmake_minimum_required(VERSION 3.8)
project(crp_msgs)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)
find_package(autoware_perception_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)


rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/TrafficRule.msg"
  "msg/PathWithTrafficRules.msg"
  "msg/World.msg"
  "msg/TargetSpace.msg"
  "msg/Scenario.msg"
  "msg/Ego.msg"
  "msg/EgoStatus.msg"
  DEPENDENCIES std_msgs geometry_msgs nav_msgs tier4_planning_msgs autoware_perception_msgs
)


ament_package()
