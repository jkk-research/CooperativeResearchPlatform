cmake_minimum_required(VERSION 3.8)
project(prcp_scenario_generator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(autoware_perception_msgs REQUIRED)
find_package(autoware_localization_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)
find_package(foxglove_msgs REQUIRED)

include_directories(include)

add_executable(prcp_scenario_generator src/scenarioGenerator.cpp)
ament_target_dependencies(prcp_scenario_generator rclcpp crp_msgs tier4_planning_msgs autoware_perception_msgs autoware_localization_msgs)

install(TARGETS
  prcp_scenario_generator
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)


ament_package()
