cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Retrieves the version numbers from "include/mantella", avoiding possible inconsistency.
file(STRINGS "./include/mantella.cmake" MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MAJOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR ${MANTELLA_HEADER_CONTENTS})
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR ${MANTELLA_HEADER_CONTENTS})
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH ${MANTELLA_HEADER_CONTENTS})
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

project(Mantella VERSION ${MANTELLA_VERSION} LANGUAGES CXX C)

# Adds additional cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(SUPPORT_OPENMP "Add OpenMP support" ON)
option(SUPPORT_MPI "Add MPI support" OFF)
option(BUILD_MEX_FILES "Build MEX files" OFF)
# Development build options
option(BUILD_TESTS "Build tests" OFF)

mark_as_advanced(
  BUILD_TESTS
)

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endif()

if (NOT INSTALL_INCLUDE_DIR)
  set(INSTALL_INCLUDE_DIR "include")
endif()

if (NOT INSTALL_LIB_DIR)
  set(INSTALL_LIB_DIR "lib")
endif()
  
message(STATUS "") 
message(STATUS "Building Mantella (version: ${MANTELLA_VERSION}).")

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "- Setting build to 'Release'.")
  message(STATUS "  - Use 'cmake ... -DCMAKE_BUILD_TYPE=[Release|Debug]' to specify this.")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# --------------------
# FINDING DEPENDENCIES
# --------------------

message(STATUS "") 
message(STATUS "Finding dependencies.")

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

if (SUPPORT_MPI)
  message(STATUS "- Adding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=OFF' to exclude this.")
  find_package(MPI REQUIRED)
else()
  message(STATUS "- Excluding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=ON' to add this.")
endif()

if (SUPPORT_OPENMP)
  message(STATUS "- Trying to add OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OPENMP=OFF' to exclude this.")

  # Makes sure that `MAXIMAL_NUMBER_OF_THREADS` contains a sane value.
  if (NOT MAXIMAL_NUMBER_OF_THREADS OR NOT MAXIMAL_NUMBER_OF_THREADS MATCHES "^[1-9][0-9]*$")
    message(STATUS "  - Trying to determine the number of cores.")
  
    include(ProcessorCount)
    ProcessorCount(MAXIMAL_NUMBER_OF_THREADS)
    message(STATUS "    - Found ${MAXIMAL_NUMBER_OF_THREADS} core(s).")
  endif()

  if (MAXIMAL_NUMBER_OF_THREADS EQUAL 0)
    message(STATUS "- Excluding OpenMP support as the maximal number of threads to support was neither automatically determined nor manually set to an integer less than 1.")
    message(STATUS "  - Use 'cmake ... -DMAXIMAL_NUMBER_OF_THREADS=...' to specify the maximal number of threads (>= 1) to support manually.")
    set(SUPPORT_OPENMP OFF)
  else()
    find_package(OpenMP)
    if (OPENMP_FOUND)
      message(STATUS "  - Adding OpenMP support (optimising for ${MAXIMAL_NUMBER_OF_THREADS} threads(s)).")
    message(STATUS "  - Use 'cmake ... -DMAXIMAL_NUMBER_OF_THREADS=...' to specify the maximal number of threads (>= 1) manually.")
      message(STATUS "  - Alternatively, set `#define MAXIMAL_NUMBER_OF_THREADS ...` in your source code, prior to including Mantella the first time.")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    else()
      message(STATUS "  - OpenMP is not supported by your compiler.")
      set(SUPPORT_OPENMP OFF)
    endif()
  endif()
else()
  message(STATUS "- Excluding OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OPENMP=ON' to add this.")
endif()

# Makes sure that `MAXIMAL_NUMBER_OF_THREADS` contains a sane value, even if OpenMP is not to be supported.
if (NOT SUPPORT_OPENMP)
  set(MAXIMAL_NUMBER_OF_THREADS 1)
endif()

# -------------------
# CONFIGURING LIBRARY
# -------------------

message(STATUS "") 
message(STATUS "Configuring library.")

# Renames configuration variables to add the `MANTELLA_` prefix.
set(MANTELLA_SUPPORT_OPENMP ${SUPPORT_OPENMP})
set(MANTELLA_SUPPORT_MPI ${SUPPORT_MPI})
set(MANTELLA_BUILD_TESTS ${BUILD_TESTS})
set(MANTELLA_MAXIMAL_NUMBER_OF_THREADS ${MAXIMAL_NUMBER_OF_THREADS})
configure_file(${PROJECT_SOURCE_DIR}/include/mantella.cmake ${PROJECT_SOURCE_DIR}/include/mantella)

# ----------------------------
# CONFIGURING UNIT TESTS BUILD
# ----------------------------

if (BUILD_TESTS)
  message(STATUS "") 
  message(STATUS "Configuring unit tests build.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=OFF' to exclude this.")
    
  # Additional requirement
  find_package(Catch REQUIRED) # Catch (header only)
  
  # All paths must start with "test/"
  add_executable(mantellaTests
    test/main.cpp
  )
  set_property(TARGET mantellaTests PROPERTY CXX_STANDARD 14)
  set_property(TARGET mantellaTests PROPERTY CXX_STANDARD_REQUIRED ON)

  message(STATUS "- Adding debugging symbols.")
  target_compile_options(mantellaTests PRIVATE -g)
  
  if (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "- Adding code coverage measurement.")
    target_compile_options(mantella PRIVATE --coverage)
    target_link_libraries(mantella PRIVATE --coverage)
  endif()
  
  message(STATUS "- Displaying compiler warnings (globally).")
  if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(
      -Wall
      -Wextra
      -pedantic-errors
      -Wno-unknown-pragmas)
  else()
    add_compile_options(
      -Weverything
      -fsanitize=undefined
      -Wno-c++98-compat
      -Wno-weak-vtables
      -Wno-exit-time-destructors
      -Wno-global-constructors
      -Wno-unknown-warning-option
      -Wno-source-uses-openmp)
  endif()
  
  message(STATUS "- Handling warnings as errors (globally).")
  add_compile_options(
    -Wno-padded
    -Wno-unused-parameter
    -Wno-c++98-compat-pedantic
    -Werror)
  
  target_include_directories(mantellaTests PUBLIC ${PROJECT_SOURCE_DIR}/include)
  
  target_link_libraries(mantellaTests PUBLIC ${BLAS_LIBRARIES})
  target_link_libraries(mantellaTests PUBLIC ${LAPACK_LIBRARIES})

  if (SUPPORT_MPI)
    target_include_directories(mantellaTests SYSTEM PUBLIC ${MPI_C_INCLUDE_PATH})
    target_link_libraries(mantellaTests PUBLIC ${MPI_C_LIBRARIES})
  endif()
  
  target_include_directories(mantellaTests PRIVATE ${CATCH_INCLUDE_DIRS})
else()
  message(STATUS "") 
  message(STATUS "Skipping unit tests build.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=ON' to add this.")
endif()

# ---------------------------
# CONFIGURING MEX FILES BUILD
# ---------------------------

if (BUILD_MEX_FILES)
  message(STATUS "") 
  message(STATUS "Configuring mex files build.")
  message(STATUS "- Use 'cmake ... -DBUILD_MEX_FILES=OFF' to exclude this.")
  
  find_package(Matlab)
  if (NOT MATLAB_FOUND)
    find_package(Octave REQUIRED)
    # Makes sure that all further addressed Matlab variables are empty.
    unset(MATLAB_MEX_FILES_DIR)
    unset(MATLAB_INCLUDE_DIR)
    unset(MATLAB_LIBRARIES)
    unset(MATLAB_LINK_OPTIONS)
    unset(MATLAB_SUFFIX)
  endif()
  
  if (NOT INSTALL_MEX_FILES_DIR)
    set(INSTALL_MEX_FILES_DIR ${MATLAB_MEX_FILES_DIR}${OCTAVE_MEX_FILES_DIR})
  endif()
  
  set_target_properties(mantella PROPERTIES POSITION_INDEPENDENT_CODE ON)
  
  add_library(mantellaMex SHARED ${PROJECT_SOURCE_DIR}/mex/mantellaMex.cpp)
  set_property(TARGET mantellaMex PROPERTY CXX_STANDARD 14)
  set_property(TARGET mantellaMex PROPERTY CXX_STANDARD_REQUIRED ON)
  target_include_directories(mantellaMex PUBLIC ${MATLAB_INCLUDE_DIR}${OCTAVE_INCLUDE_DIR})
  target_link_libraries(mantellaMex PUBLIC ${MATLAB_LIBRARIES}${OCTAVE_LIBRARIES})
  target_link_libraries(mantellaMex PUBLIC mantella)
  install(TARGETS mantellaMex DESTINATION ${INSTALL_LIB_DIR})
  
  file(GLOB MEX_FILES ${PROJECT_SOURCE_DIR}/mex/*.cpp)
  list(REMOVE_ITEM MEX_FILES ${PROJECT_SOURCE_DIR}/mex/mantellaMex.cpp)
  foreach(MEX_FILE ${MEX_FILES})
    get_filename_component(MEX_FILENAME ${MEX_FILE} NAME_WE)
    add_library(${MEX_FILENAME} SHARED ${MEX_FILE})
    set_property(TARGET ${MEX_FILENAME} PROPERTY CXX_STANDARD 14)
    set_property(TARGET ${MEX_FILENAME} PROPERTY CXX_STANDARD_REQUIRED ON)
    target_link_libraries(${MEX_FILENAME} PRIVATE mantellaMex)
    target_link_libraries(${MEX_FILENAME} PRIVATE ${MATLAB_LINK_OPTIONS}${OCTAVE_LINK_OPTIONS})
    set_target_properties(${MEX_FILENAME} PROPERTIES PREFIX "" OUTPUT_NAME ${MEX_FILENAME} SUFFIX ${MATLAB_SUFFIX}${OCTAVE_SUFFIX})
    install(TARGETS ${MEX_FILENAME} DESTINATION ${INSTALL_MEX_FILES_DIR})
  endforeach()
else()
  message(STATUS "") 
  message(STATUS "Skipping mex files build.")
  message(STATUS "- Use 'cmake ... -DBUILD_MEX_FILES=ON' to add this.")
endif()

# --------------------------------
# CONFIGURING LIBRARY INSTALLATION
# --------------------------------

message(STATUS "")
message(STATUS "Configuring library installation.")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)
  
message(STATUS "")
message(STATUS "Noticable variables:")
message(STATUS "- CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message(STATUS "- CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "- SUPPORT_OPENMP = ${SUPPORT_OPENMP}")
message(STATUS "- SUPPORT_MPI = ${SUPPORT_MPI}")
message(STATUS "- INSTALL_INCLUDE_DIR = ${INSTALL_INCLUDE_DIR}")
message(STATUS "- INSTALL_LIB_DIR = ${INSTALL_LIB_DIR}")
message(STATUS "- INSTALL_MEX_FILES_DIR = ${INSTALL_MEX_FILES_DIR}")
message(STATUS "")
