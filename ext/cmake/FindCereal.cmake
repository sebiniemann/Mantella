find_path(CEREAL_INCLUDE_DIRS cereal/cereal.hpp
  PATHS "$ENV{ProgramFiles}/Cereal/include")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cereal DEFAULT_MSG CEREAL_INCLUDE_DIRS)

mark_as_advanced(CEREAL_INCLUDE_DIRS)

