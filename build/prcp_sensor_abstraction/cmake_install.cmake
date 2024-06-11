# Install script for directory: /home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/crp_CIL/prcp_sensor_abstraction

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/install/prcp_sensor_abstraction")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction" TYPE EXECUTABLE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/world_abstraction")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction"
         OLD_RPATH "/opt/ros/humble/lib:/home/gfigneczi1/autoware/install/autoware_perception_msgs/lib:/home/gfigneczi1/autoware/install/autoware_map_msgs/lib:/home/gfigneczi1/autoware/install/tier4_planning_msgs/lib:/home/gfigneczi1/autoware/install/autoware_common_msgs/lib:/home/gfigneczi1/autoware/install/autoware_planning_msgs/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/world_abstraction")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction" TYPE EXECUTABLE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/scenario_abstraction")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction"
         OLD_RPATH "/home/gfigneczi1/autoware/install/autoware_map_msgs/lib:/home/gfigneczi1/autoware/install/autoware_perception_msgs/lib:/home/gfigneczi1/autoware/install/tier4_planning_msgs/lib:/home/gfigneczi1/autoware/install/autoware_planning_msgs/lib:/opt/ros/humble/lib:/home/gfigneczi1/autoware/install/autoware_utils/lib:/opt/ros/humble/lib/aarch64-linux-gnu:/home/gfigneczi1/autoware/install/autoware_lanelet2_extension/lib:/home/gfigneczi1/autoware/install/autoware_common_msgs/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/scenario_abstraction")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction" TYPE EXECUTABLE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ego_abstraction")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction"
         OLD_RPATH "/opt/ros/humble/lib:/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/install/crp_msgs/lib:/home/gfigneczi1/autoware/install/autoware_localization_msgs/lib:/home/gfigneczi1/autoware/install/tier4_planning_msgs/lib:/home/gfigneczi1/autoware/install/autoware_common_msgs/lib:/home/gfigneczi1/autoware/install/autoware_planning_msgs/lib:/home/gfigneczi1/autoware/install/autoware_perception_msgs/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/prcp_sensor_abstraction/ego_abstraction")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/package_run_dependencies" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_index/share/ament_index/resource_index/package_run_dependencies/prcp_sensor_abstraction")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/parent_prefix_path" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_index/share/ament_index/resource_index/parent_prefix_path/prcp_sensor_abstraction")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction/environment" TYPE FILE FILES "/home/gfigneczi1/autoware/install/ament_cmake_core/share/ament_cmake_core/cmake/environment_hooks/environment/ament_prefix_path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction/environment" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/ament_prefix_path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction/environment" TYPE FILE FILES "/home/gfigneczi1/autoware/install/ament_cmake_core/share/ament_cmake_core/cmake/environment_hooks/environment/path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction/environment" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/local_setup.bash")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/local_setup.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/local_setup.zsh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/local_setup.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_environment_hooks/package.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_index/share/ament_index/resource_index/packages/prcp_sensor_abstraction")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction/cmake" TYPE FILE FILES
    "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_core/prcp_sensor_abstractionConfig.cmake"
    "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/ament_cmake_core/prcp_sensor_abstractionConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/prcp_sensor_abstraction" TYPE FILE FILES "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/crp_CIL/prcp_sensor_abstraction/package.xml")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/gfigneczi1/ros2_ws/src/CooperativeResearchPlatform/build/prcp_sensor_abstraction/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
