cmake_minimum_required(VERSION 3.8)
project(ctrl_vehicle_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

include_directories(include)

add_executable(control_handler src/control_handler.cpp)

ament_target_dependencies(control_handler rclcpp geometry_msgs std_msgs crp_msgs autoware_control_msgs)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS
  control_handler
  DESTINATION lib/${PROJECT_NAME})

ament_package()
