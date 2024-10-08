cmake_minimum_required(VERSION 3.8)
project(plan_behavior_planning)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)


include_directories(include)

add_executable(mission_planner src/missionPlanner.cpp)
ament_target_dependencies(mission_planner rclcpp crp_msgs autoware_planning_msgs tier4_planning_msgs)

add_executable(behavior_planner src/behaviorPlanner.cpp)
ament_target_dependencies(behavior_planner rclcpp crp_msgs autoware_planning_msgs tier4_planning_msgs)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS
  mission_planner
  behavior_planner
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
