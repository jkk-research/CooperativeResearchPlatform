cmake_minimum_required(VERSION 3.8)
project(duro_gps_wrapper)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

include_directories(include)

add_executable(duro_topic_converter src/duroTopicConverter.cpp)
ament_target_dependencies(duro_topic_converter rclcpp geometry_msgs tf2_geometry_msgs)

install(TARGETS
  duro_topic_converter
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch 
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
