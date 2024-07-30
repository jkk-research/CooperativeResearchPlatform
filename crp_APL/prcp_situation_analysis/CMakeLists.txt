cmake_minimum_required(VERSION 3.8)
project(prcp_situation_analysis)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(autoware_map_msgs REQUIRED)
find_package(autoware_perception_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)


include_directories(include)

add_executable(input_fusion src/inputFusion.cpp)
ament_target_dependencies(input_fusion rclcpp crp_msgs nav_msgs autoware_map_msgs autoware_perception_msgs tier4_planning_msgs)

install(TARGETS
  input_fusion
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
