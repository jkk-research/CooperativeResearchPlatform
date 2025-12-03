cmake_minimum_required(VERSION 3.8)
project(radar_pointcloud_processor)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

include_directories(include)

add_executable(doppler_compensator src/dopplerCompensator.cpp)
ament_target_dependencies(doppler_compensator rclcpp sensor_msgs geometry_msgs tf2_ros)

add_executable(pointcloud_merger src/pointcloudMerger.cpp)
ament_target_dependencies(pointcloud_merger rclcpp sensor_msgs)


install(TARGETS
  doppler_compensator
  pointcloud_merger
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  config
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
