cmake_minimum_required(VERSION 3.8)
project(actuator_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(pacmod3_msgs REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(pdp_if REQUIRED)
find_package(autoware_control_msgs REQUIRED)

include_directories(include)

add_executable(actuator_control src/actuator_control.cpp)
ament_target_dependencies(actuator_control rclcpp std_msgs pacmod3_msgs crp_msgs pdp_if autoware_control_msgs)

install(TARGETS
actuator_control
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
