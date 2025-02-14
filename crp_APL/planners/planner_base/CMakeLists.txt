cmake_minimum_required(VERSION 3.8)
project(planner_base)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(tier4_planning_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)
find_package(autoware_perception_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

set (
  INTERFACES_DIR
  ${CMAKE_SOURCE_DIR}/../../interfaces
)

set (
  UTILS_DIR
  ${CMAKE_SOURCE_DIR}/../../crp_utils
)

include_directories(
  include
  ${INTERFACES_DIR}
  ${UTILS_DIR}
)

set(utils_source
${CMAKE_CURRENT_SOURCE_DIR}/../../crp_utils/geometryUtils/src/geometricPathCalculation.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../../crp_utils/geometryUtils/src/polynomialRegression.cpp)

add_library(plan_wrapper_base SHARED
  src/planWrapperBase.cpp ${utils_source}
)
ament_target_dependencies(plan_wrapper_base rclcpp crp_msgs tier4_planning_msgs autoware_planning_msgs autoware_perception_msgs tf2 tf2_geometry_msgs nav_msgs)

install(
  DIRECTORY include/
  DESTINATION include/
)

install(
  TARGETS plan_wrapper_base
  EXPORT plan_wrapper_base
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

ament_export_include_directories(${INTERFACES_DIR})
ament_export_include_directories(${UTILS_DIR})
ament_export_targets(plan_wrapper_base HAS_LIBRARY_TARGET)
ament_export_dependencies(rclcpp crp_msgs tier4_planning_msgs autoware_planning_msgs tf2 tf2_geometry_msgs)

ament_package()
