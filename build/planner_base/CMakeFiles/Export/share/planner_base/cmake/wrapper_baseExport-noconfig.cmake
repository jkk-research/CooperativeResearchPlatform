#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "planner_base::wrapper_base" for configuration ""
set_property(TARGET planner_base::wrapper_base APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(planner_base::wrapper_base PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libwrapper_base.so"
  IMPORTED_SONAME_NOCONFIG "libwrapper_base.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS planner_base::wrapper_base )
list(APPEND _IMPORT_CHECK_FILES_FOR_planner_base::wrapper_base "${_IMPORT_PREFIX}/lib/libwrapper_base.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
