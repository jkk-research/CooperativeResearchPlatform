cmake_minimum_required(VERSION 3.8)
project(plan_lon_emergency)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(planner_base REQUIRED)

include_directories(include ${planner_base_INCLUDE_DIRS})


add_executable(plan_lon_emergency src/wrapperPlanLonEmergency.cpp)
ament_target_dependencies(plan_lon_emergency rclcpp planner_base)

install(TARGETS
plan_lon_emergency
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
