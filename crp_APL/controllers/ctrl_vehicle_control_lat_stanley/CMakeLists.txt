cmake_minimum_required(VERSION 3.8)
project(ctrl_vehicle_control_lat_stanley)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)
find_package(autoware_vehicle_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(crp_msgs REQUIRED)


include_directories(include)


add_executable(ctrl_vehicle_control_lat_stanley src/ctrl_vehicle_control_lat_stanley.cpp)

ament_target_dependencies(ctrl_vehicle_control_lat_stanley  rclcpp geometry_msgs std_msgs autoware_control_msgs autoware_planning_msgs autoware_vehicle_msgs nav_msgs crp_msgs)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS
ctrl_vehicle_control_lat_stanley
  DESTINATION lib/${PROJECT_NAME})

ament_package()
