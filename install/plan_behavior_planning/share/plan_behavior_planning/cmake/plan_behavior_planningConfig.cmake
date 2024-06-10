# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_plan_behavior_planning_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED plan_behavior_planning_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(plan_behavior_planning_FOUND FALSE)
  elseif(NOT plan_behavior_planning_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(plan_behavior_planning_FOUND FALSE)
  endif()
  return()
endif()
set(_plan_behavior_planning_CONFIG_INCLUDED TRUE)

# output package information
if(NOT plan_behavior_planning_FIND_QUIETLY)
  message(STATUS "Found plan_behavior_planning: 0.0.0 (${plan_behavior_planning_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'plan_behavior_planning' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${plan_behavior_planning_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(plan_behavior_planning_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${plan_behavior_planning_DIR}/${_extra}")
endforeach()
