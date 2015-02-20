find_path(MANTELLA_INCLUDE_DIRS mantella
  PATHS "$ENV{ProgramFiles}/Mantella")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Mantella DEFAULT_MSG MANTELLA_INCLUDE_DIRS)

mark_as_advanced(MANTELLA_INCLUDE_DIRS)

