cmake_minimum_required(VERSION 3.8)
project(cloud_filter)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pcl_ros)
find_package (std_msgs)
find_package(visualization_msgs)
find_package(tf2_ros)
find_package(pcl_conversions)

find_package(Eigen3 REQUIRED)
find_package(PCL 1.8 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem thread system program_options)

set(INCLUDE_DIRS 
  include 
  ${ament_cmake_INCLUDE_DIRS} 
  ${rclcpp_INCLUDE_DIRS}
  ${pcl_ros_INCLUDE_DIRS} 
  ${pcl_conversions_INCLUDE_DIRS}
  ${std_msgs_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${nav_msgs_INCLUDE_DIRS}
  ${rclcpp_components_INCLUDE_DIRS}
)
include_directories(${INCLUDE_DIRS})


  
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

set(ament_dependencies
  pcl_conversions
  rclcpp
  std_msgs
  sensor_msgs
  nav_msgs
  visualization_msgs
  tf2
  tf2_msgs
  tf2_ros
  tf2_geometry_msgs  
  PCL
)




add_executable(cloud_filter src/cloud_filter.cpp)
target_link_libraries(cloud_filter ${PCL_LIBRARIES})
ament_target_dependencies(cloud_filter rclcpp  visualization_msgs tf2_ros pcl_ros pcl_conversions std_msgs Eigen3 PCL Boost)

install(TARGETS
  cloud_filter
  DESTINATION lib/${PROJECT_NAME}
)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
