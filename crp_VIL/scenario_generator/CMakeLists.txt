cmake_minimum_required(VERSION 3.8)
project(scenario_generator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)

include_directories(include)

add_executable(scenario_gen src/scenarioGen.cpp)
ament_target_dependencies(scenario_gen rclcpp crp_msgs)

install(TARGETS
  scenario_gen
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
