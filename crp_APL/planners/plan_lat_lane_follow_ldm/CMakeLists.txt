cmake_minimum_required(VERSION 3.8)
project(plan_lat_lane_follow_ldm)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(planner_base REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../crp_utils)

set(functional_source
   ${CMAKE_CURRENT_SOURCE_DIR}/src/functionCode/src/linearDriverModel_controlLogic.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/functionCode/src/linearDriverModel.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/functionCode/src/linearDriverModelDriverModel/linearDriverModel_driverModel.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/functionCode/src/linearDriverModelPlanner/linearDriverModel_segmentPlanner.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/functionCode/src/linearDriverModelUtilities/linearDriverModel_coordinateTransforms.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/functionCode/src/linearDriverModelUtilities/linearDriverModel_polynomialSubfunctions.cpp
)

set(utils_source
${CMAKE_CURRENT_SOURCE_DIR}/../../crp_utils/geometryUtils/src/polynomialRegression.cpp)

add_executable(plan_lat_lane_follow_ldm src/wrapperPlanLatLaneFollowLdm.cpp ${functional_source} ${utils_source})
ament_target_dependencies(plan_lat_lane_follow_ldm rclcpp planner_base)

install(TARGETS
plan_lat_lane_follow_ldm
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
