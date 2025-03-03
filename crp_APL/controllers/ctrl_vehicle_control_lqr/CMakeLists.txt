cmake_minimum_required(VERSION 3.8)
project(ctrl_vehicle_control_lqr)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(crp_msgs REQUIRED)


install(
  PROGRAMS
    scripts/lqr_controller.py
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY
  config
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
