cmake_minimum_required(VERSION 3.8)
project(ctrl_vehicle_control_long)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)


include_directories(include)


add_executable(ctrl_vehicle_control_long src/ctrl_vehicle_control_long.cpp)

ament_target_dependencies(ctrl_vehicle_control_long  rclcpp autoware_control_msgs autoware_planning_msgs crp_msgs)

install(DIRECTORY
  config
  launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS
ctrl_vehicle_control_long
  DESTINATION lib/${PROJECT_NAME})

ament_package()
