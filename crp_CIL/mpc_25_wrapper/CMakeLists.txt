cmake_minimum_required(VERSION 3.8)
project(mpc_25_wrapper)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tier4_planning_msgs REQUIRED)
find_package(crp_if REQUIRED)

include_directories(include)

add_executable(mpc_25_wrapper src/mpc_25_wrapper.cpp)
ament_target_dependencies(mpc_25_wrapper rclcpp tier4_planning_msgs crp_if)

install(TARGETS
  mpc_25_wrapper
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch 
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
