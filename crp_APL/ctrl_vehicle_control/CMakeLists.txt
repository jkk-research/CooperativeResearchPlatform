cmake_minimum_required(VERSION 3.8)
project(ctrl_vehicle_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(autoware_auto_control_msgs REQUIRED)
find_package(autoware_auto_planning_msgs REQUIRED)
find_package(autoware_auto_vehicle_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(pacmod3_msgs REQUIRED)



set(ament_dependencies
  rclcpp
  geometry_msgs
  std_msgs
  autoware_auto_control_msgs
  autoware_auto_planning_msgs
  autoware_auto_vehicle_msgs
  nav_msgs
  pacmod3_msgs
)


add_executable(ctrl_vehicle_control src/controller_cpp/ctrl_vehicle_control.cpp src/controller_cpp/lib/utils.cpp
  src/controller_cpp/compensatory_model/compensatory_model.cpp
  src/controller_cpp/lib/polynomialCalculator.cpp )

add_executable(plan_waypoint_publisher src/controller_cpp/helper_nodes/waypoint_publisher.cpp
    src/controller_cpp/lib/utils.cpp)

add_executable(model_vehicle_model src/controller_cpp/helper_nodes/vehicle_model.cpp
    src/controller_cpp/lib/utils.cpp)

ament_target_dependencies(ctrl_vehicle_control  ${ament_dependencies})
ament_target_dependencies(plan_waypoint_publisher  ${ament_dependencies})
ament_target_dependencies(model_vehicle_model  ${ament_dependencies})

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS ctrl_vehicle_control
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS plan_waypoint_publisher
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS model_vehicle_model
  DESTINATION lib/${PROJECT_NAME})

ament_package()
