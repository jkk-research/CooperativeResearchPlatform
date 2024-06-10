# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_prcp_situation_analysis_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED prcp_situation_analysis_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(prcp_situation_analysis_FOUND FALSE)
  elseif(NOT prcp_situation_analysis_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(prcp_situation_analysis_FOUND FALSE)
  endif()
  return()
endif()
set(_prcp_situation_analysis_CONFIG_INCLUDED TRUE)

# output package information
if(NOT prcp_situation_analysis_FIND_QUIETLY)
  message(STATUS "Found prcp_situation_analysis: 0.0.0 (${prcp_situation_analysis_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'prcp_situation_analysis' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${prcp_situation_analysis_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(prcp_situation_analysis_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${prcp_situation_analysis_DIR}/${_extra}")
endforeach()
