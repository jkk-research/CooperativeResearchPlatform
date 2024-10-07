cmake_minimum_required(VERSION 3.8)
project(lanelet_handler)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(autoware_map_msgs REQUIRED)
find_package(map_loader REQUIRED)
find_package(lanelet2_core REQUIRED)
find_package(lanelet2_projection REQUIRED)
find_package(lanelet2_io REQUIRED)
find_package(autoware_lanelet2_extension REQUIRED)

include_directories(
  include
)

add_executable(lanelet_file_loader src/laneletFileLoader.cpp)
ament_target_dependencies(lanelet_file_loader rclcpp autoware_map_msgs lanelet2_core lanelet2_projection lanelet2_io autoware_lanelet2_extension)

install(TARGETS
  lanelet_file_loader
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
