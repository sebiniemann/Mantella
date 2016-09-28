cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Retrieves the version numbers from "include/mantella", avoiding possible inconsistency.
file(STRINGS "../include/mantella" MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MAJOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH "${MANTELLA_HEADER_CONTENTS}")
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

project(Mantella VERSION ${MANTELLA_VERSION} LANGUAGES CXX C)

option(USE_OPENMP "Use OpenMP within tests" OFF)
option(USE_MPI "Use MPI within tests" OFF)

message(STATUS "") 
message(STATUS "Configuring tests for Mantella (version: ${MANTELLA_VERSION}).")

find_path(MANTELLA_INCLUDE_DIR mantella${MANTELLA_VERSION_MAJOR})
find_path(CATCH_INCLUDE_DIR catch.hpp)

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

if (USE_OPENMP)
  find_package(OpenMP REQUIRED)
endif()

if (USE_MPI)
  find_package(MPI REQUIRED)
endif()

add_executable(tests main.cpp)
set_property(TARGET tests PROPERTY CXX_STANDARD 14)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

message(STATUS "- Adding debugging symbols.")
target_compile_options(tests PRIVATE -g)

if (CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "- Adding code coverage measurement.")
  target_compile_options(mantella PRIVATE --coverage)
  target_link_libraries(mantella PRIVATE --coverage)
endif()

message(STATUS "- Displaying compiler warnings.")
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

message(STATUS "- Handling warnings as errors.")
add_compile_options(
  -Wno-padded
  -Wno-unused-parameter
  -Wno-c++98-compat-pedantic
  -Werror)
  
if (USE_OPENMP)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

target_include_directories(tests PRIVATE ${MANTELLA_INCLUDE_DIR})
target_include_directories(tests PRIVATE ${CATCH_INCLUDE_DIR})
if (USE_MPI)
  target_include_directories(tests SYSTEM PRIVATE ${MPI_C_INCLUDE_PATH})
endif()

target_link_libraries(tests PRIVATE ${BLAS_LIBRARIES})
target_link_libraries(tests PRIVATE ${LAPACK_LIBRARIES})
if (USE_MPI)
  target_link_libraries(tests PRIVATE ${MPI_C_LIBRARIES})
endif()

#
# Prints noticeable variables
#

message(STATUS "")
message(STATUS "Noticable variables:")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "- USE_OPENMP = ${USE_OPENMP}")
message(STATUS "- USE_MPI = ${USE_MPI}")
message(STATUS "")
