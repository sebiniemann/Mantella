cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(BenchmarkMantella CXX)

message(STATUS "") 
message(STATUS "Configuring benchmark for Mantella.")

add_executable(benchmark benchmark.cpp)
set_property(TARGET benchmark PROPERTY CXX_STANDARD 14)
set_property(TARGET benchmark PROPERTY CXX_STANDARD_REQUIRED ON)

add_compile_options(
  -O3
  -march=native
  -ffast-math)

find_path(MANTELLA_INCLUDE_DIR mantella0)
target_include_directories(benchmark SYSTEM PRIVATE ${MANTELLA_INCLUDE_DIR})

#
# Prints noticeable variables
#

message(STATUS "")
message(STATUS "Noticable variables:")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "")
