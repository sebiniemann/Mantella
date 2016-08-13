find_library(OPENBLAS_LIBRARIES openblas)

# The pre-build OpenBLAS libraries depend on `libgfortran.a` and `libquadmath.a`
if (WIN32 AND MINGW)
  find_library(OPENBLAS_GFORTAN gfortran)
  list(APPEND OPENBLAS_LIBRARIES ${OPENBLAS_GFORTAN})
  find_library(OPENBLAS_QUADMATH quadmath)
  list(APPEND OPENBLAS_LIBRARIES ${OPENBLAS_QUADMATH})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenBLAS
  FOUND_VAR OPENBLAS_FOUND
  REQUIRED_VARS OPENBLAS_LIBRARIES)

mark_as_advanced(
  OPENBLAS_FOUND
  OPENBLAS_LIBRARIES)