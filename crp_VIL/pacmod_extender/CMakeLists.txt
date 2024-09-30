cmake_minimum_required(VERSION 3.8)
project(pacmod_extender)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pacmod3_msgs REQUIRED)
find_package(can_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

include_directories(
  include
)

add_executable(pacmod_extender_node src/pacmodExtender.cpp src/pacmodDefinitions.cpp)
ament_target_dependencies(pacmod_extender_node rclcpp pacmod3_msgs can_msgs geometry_msgs)

install(TARGETS
  pacmod_extender_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch 
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
