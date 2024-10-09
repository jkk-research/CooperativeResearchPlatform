cmake_minimum_required(VERSION 3.8)
project(test_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)


include_directories(include)

include_directories(../interfaces)

add_executable(test_node src/testNode.cpp)
ament_target_dependencies(test_node rclcpp crp_msgs autoware_planning_msgs autoware_control_msgs tier4_planning_msgs tf2 tf2_geometry_msgs)

install(TARGETS
  test_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
