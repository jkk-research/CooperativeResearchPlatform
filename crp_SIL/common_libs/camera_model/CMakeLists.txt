cmake_minimum_required(VERSION 3.14)
project(camera_model)

# Set the C++ standard to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# General C++ compile options
add_compile_options(
  -Ofast        # Enables -O3 optimizations plus some non-standard ones
  -fconcepts    # Enables C++20 concepts support
)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(PCL REQUIRED)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIRS}
)

# Create the library
add_library(${PROJECT_NAME} src/camera_model.cpp)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen ${PCL_LIBRARIES})
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  sensor_msgs
  Eigen3
  PCL
)

# Install the library target
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install header files
install(
  DIRECTORY include/
  DESTINATION include
)

# Export the library for downstream packages
ament_export_include_directories(include)
ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)
ament_export_dependencies(
  rclcpp
  sensor_msgs
  Eigen3
  PCL
)
ament_package()
