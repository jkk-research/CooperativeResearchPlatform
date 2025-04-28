cmake_minimum_required(VERSION 3.14)
project(prcp_grid_cam)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# General C++ compile options
add_compile_options(
  -Ofast            # Enables all -O3 optimizations plus non-standard-compliant optimizations
  -fconcepts        # Enables support for C++20 concepts
)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
# find_package(PCL REQUIRED)
find_package(camera_model REQUIRED)


set(ament_dependencies
  rclcpp
  std_msgs
  sensor_msgs
  nav_msgs
  tf2_ros
  tf2_geometry_msgs
  cv_bridge
  OpenCV
  camera_model
)


add_executable(prcp_grid_cam_node src/prcp_grid_cam_node.cpp)
target_include_directories(prcp_grid_cam_node PRIVATE ${camera_model_INCLUDE_DIRS})
ament_target_dependencies(prcp_grid_cam_node ${ament_dependencies})


install(TARGETS
  prcp_grid_cam_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
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
