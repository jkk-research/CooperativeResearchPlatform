cmake_minimum_required(VERSION 3.8)
project(controller_base)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(crp_msgs REQUIRED)
find_package(autoware_planning_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)

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

add_library(ctrl_lat_wrapper_base SHARED
  src/ctrlLatWrapperBase.cpp src/ctrlWrapperBase.cpp ${UTILS_DIR}/generalUtils/src/dataConversion.cpp 
)
ament_target_dependencies(ctrl_lat_wrapper_base rclcpp crp_msgs autoware_planning_msgs autoware_control_msgs)

add_library(ctrl_long_wrapper_base SHARED
  src/ctrlLongWrapperBase.cpp src/ctrlWrapperBase.cpp ${UTILS_DIR}/generalUtils/src/dataConversion.cpp
)
ament_target_dependencies(ctrl_long_wrapper_base rclcpp crp_msgs autoware_planning_msgs autoware_control_msgs)

install(
  DIRECTORY include/
  DESTINATION include/
)

install(
  TARGETS ctrl_lat_wrapper_base
  EXPORT ctrl_lat_wrapper_base
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  TARGETS ctrl_long_wrapper_base
  EXPORT ctrl_long_wrapper_base
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

ament_export_include_directories(${INTERFACES_DIR})
ament_export_include_directories(${UTILS_DIR})
ament_export_targets(ctrl_lat_wrapper_base HAS_LIBRARY_TARGET)
ament_export_targets(ctrl_long_wrapper_base HAS_LIBRARY_TARGET)
ament_export_dependencies(rclcpp)


ament_package()
