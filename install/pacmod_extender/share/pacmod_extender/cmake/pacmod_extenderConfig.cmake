# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_pacmod_extender_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED pacmod_extender_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(pacmod_extender_FOUND FALSE)
  elseif(NOT pacmod_extender_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(pacmod_extender_FOUND FALSE)
  endif()
  return()
endif()
set(_pacmod_extender_CONFIG_INCLUDED TRUE)

# output package information
if(NOT pacmod_extender_FIND_QUIETLY)
  message(STATUS "Found pacmod_extender: 0.0.0 (${pacmod_extender_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'pacmod_extender' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${pacmod_extender_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(pacmod_extender_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${pacmod_extender_DIR}/${_extra}")
endforeach()
