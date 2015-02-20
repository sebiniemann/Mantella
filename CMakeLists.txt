#------------------------------------------------------------------------------
# Copyright 2013-2015 Sebastian Niemann <niemann@sra.uni-hannover.de>.
#
# Licensed under the MIT License (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://opensource.org/licenses/MIT
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Mantella examples
project(MantellaExamples CXX)

# Add additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Build options
option(RUN_EXAMPLES "Run examples" OFF)

# Set output directories
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
endif()

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Display all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Use native machine features (SSE3, SSE4, AVX, ...), if possible and compiled with Armadillo
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

# --------------
# BUILD EXAMPLES
# --------------

message(STATUS "NOTICE: Configuring examples")

# Locate Mantella and third party libraries and headers
find_package(Armadillo 4.600.0 REQUIRED) # Armadillo C++
find_package(Mantella REQUIRED) # Mantella (header only)

# Include Mantella and third party headers
include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
include_directories(${MANTELLA_INCLUDE_DIR}) # Mantella tests

file(GLOB EXAMPLES RELATIVE _include/*.cpp)
foreach(exampleFile ${EXAMPLES} )
  string(REPLACE ".cpp" "" exampleName ${exampleFile})
  add_executable( ${exampleName} ${exampleFile} )
    
  # Link against third party libraries
  target_link_libraries(${exampleName} ${ARMADILLO_LIBRARIES}) # Armadillo C++
endforeach()

if(RUN_EXAMPLES)
  message(STATUS "NOTICE: Automatically running all examples after building them.")

  add_custom_command(TARGET examples POST_BUILD COMMAND libraryTest ${PROJECT_SOURCE_DIR}/test)
endif()