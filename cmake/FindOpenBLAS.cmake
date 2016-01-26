if(WIN32)
  # The precompiled OpenBLAS (import) libraries for Windows are called *libopenblas.a* (static) and *libopenblas.dll.a*.
  list(APPEND CMAKE_FIND_LIBRARY_PREFIXES "lib")
  list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a")
endif()

find_library(OPENBLAS_LIBRARIES openblas)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenBLAS DEFAULT_MSG OPENBLAS_LIBRARIES)

mark_as_advanced(OPENBLAS_LIBRARIES)