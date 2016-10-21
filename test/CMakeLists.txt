cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Retrieves the version numbers from "include/mantella", avoiding possible inconsistency.
file(GLOB MANTELLA_HEADER_FILE "../include/mantella*")
string(REGEX REPLACE ".*/include/mantella([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR "${MANTELLA_HEADER_FILE}")
file(STRINGS ../include/mantella${MANTELLA_VERSION_MAJOR} MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH "${MANTELLA_HEADER_CONTENTS}")
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

project(MantellaTests VERSION ${MANTELLA_VERSION} LANGUAGES CXX)

message(STATUS "") 
message(STATUS "Configuring tests for Mantella (version: ${MANTELLA_VERSION}).")

configure_file(${PROJECT_SOURCE_DIR}/main.cpp.cmake ${PROJECT_SOURCE_DIR}/main.cpp)
add_executable(tests main.cpp)
set_property(TARGET tests PROPERTY CXX_STANDARD 14)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

if (NOT MSVC)
  message(STATUS "- Adding debugging symbols.")
  target_compile_options(tests PRIVATE -g)
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
add_compile_options(
  -Wno-padded
  -Wno-unused-parameter
  -Wno-c++98-compat-pedantic)

message(STATUS "- Handling warnings as errors.")
add_compile_options(
  -Werror)

target_include_directories(tests PRIVATE "../include")

find_path(CATCH_INCLUDE_DIR catch.hpp)
target_include_directories(tests SYSTEM PRIVATE ${CATCH_INCLUDE_DIR})

#
# Prints noticeable variables
#

message(STATUS "")
message(STATUS "Noticable variables:")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "")
