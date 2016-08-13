find_path(CATCH_INCLUDE_DIR catch.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Catch 
  FOUND_VAR CATCH_FOUND
  REQUIRED_VARS CATCH_INCLUDE_DIR)

mark_as_advanced(
  CATCH_FOUND
  CATCH_INCLUDE_DIR)

