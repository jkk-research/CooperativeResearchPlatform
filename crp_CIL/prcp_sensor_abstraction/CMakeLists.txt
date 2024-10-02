cmake_minimum_required(VERSION 3.8)
project(prcp_sensor_abstraction)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(autoware_perception_msgs REQUIRED)
find_package(autoware_localization_msgs REQUIRED)
find_package(autoware_map_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(lanelet2_core REQUIRED)
find_package(autoware_lanelet2_extension REQUIRED)
find_package(crp_msgs REQUIRED)

include_directories(include)

add_executable(world_abstraction src/worldAbstraction.cpp)
ament_target_dependencies(world_abstraction rclcpp nav_msgs autoware_perception_msgs autoware_map_msgs tier4_planning_msgs) 

add_executable(scenario_abstraction src/scenarioAbstraction.cpp)
ament_target_dependencies(scenario_abstraction rclcpp nav_msgs autoware_map_msgs autoware_perception_msgs tier4_planning_msgs geometry_msgs lanelet2_core autoware_lanelet2_extension)

add_executable(ego_abstraction src/egoAbstraction.cpp)
ament_target_dependencies(ego_abstraction rclcpp autoware_localization_msgs tier4_planning_msgs)
ament_target_dependencies(ego_abstraction rclcpp crp_msgs geometry_msgs nav_msgs autoware_localization_msgs)

install(TARGETS
  world_abstraction
  scenario_abstraction
  ego_abstraction
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
