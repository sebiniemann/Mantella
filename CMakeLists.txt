#------------------------------------------------------------------------------
# Copyright 2013-2014 Sebastian Niemann <niemann@sra.uni-hannover.de>.
#
# Licensed under the MIT License (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://opensource.org/licenses/MIT
#
# Authors:
#   Sebastian Niemann - Lead developer
#   Wolff Bock von Wülfingen - Developer
#   Daniel Kiechle - Developer / Software tester
#   Oskar Pusz - Developer
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

# Hint-based Online Programming (HOP)
project(HOP CXX)

# Add additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ext/cmake)

# Provide custom build management options
option(BUILD_LIBRARY "Build library" ON)
option(BUILD_GUI "Build GUI" OFF)
option(BUILD_CLI "Build CLI" OFF)
option(BUILD_ALL "Build all" OFF)

if(BUILD_ALL)
  set(BUILD_LIBRARY ON)
  set(BUILD_GUI ON)
  set(BUILD_CLI ON)
endif()

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# Provide support for C++11 features
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

# Include HOP library headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# -------------
# BUILD LIBRARY
# -------------
if(BUILD_LIBRARY)
  message(STATUS "NOTICE: Configuring Hint-based Online Programming (HOP) library build.")
  
  # Locate libraries
  find_package(Armadillo REQUIRED)
  
  # Include headers
  include_directories(${ARMADILLO_INCLUDE_DIRS})
  
  # Compile the optimisation library
  # All paths must start with "src/"
  add_library(hop SHARED
    src/optimisationProblem/optimisationProblem.cpp
    src/optimisationProblem/benchmark/benchmarkProblem.cpp
    src/optimisationProblem/benchmark/attractiveSectorFunction.cpp
    src/optimisationProblem/benchmark/bentCigarFunction.cpp
    src/optimisationProblem/benchmark/buecheRastriginFunction.cpp
    src/optimisationProblem/benchmark/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/benchmark/differentPowersFunction.cpp
    src/optimisationProblem/benchmark/discusFunction.cpp
    src/optimisationProblem/benchmark/ellipsoidalFunction.cpp
    src/optimisationProblem/benchmark/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/benchmark/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/benchmark/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/benchmark/katsuuraFunction.cpp
    src/optimisationProblem/benchmark/linearSlope.cpp
    src/optimisationProblem/benchmark/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/benchmark/rastriginFunction.cpp
    src/optimisationProblem/benchmark/rastriginFunctionRotated.cpp
    src/optimisationProblem/benchmark/rosenbrockFunction.cpp
    src/optimisationProblem/benchmark/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/benchmark/schaffersF7Function.cpp
    src/optimisationProblem/benchmark/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/benchmark/schwefelFunction.cpp
    src/optimisationProblem/benchmark/sharpRidgeFunction.cpp
    src/optimisationProblem/benchmark/sphereFunction.cpp
    src/optimisationProblem/benchmark/stepEllipsoidalFunction.cpp
    src/optimisationProblem/benchmark/weierstrassFunction.cpp
    src/helper/random.cpp
  )
  target_link_libraries (hop ${ARMADILLO_LIBRARIES})
endif()

# ---------
# BUILD GUI
# ---------
if(BUILD_GUI)
  message(STATUS "")
  message(STATUS "ERROR: Building the GUI via cmake is not supported. Use qmake instead.")
  message(STATUS "")
endif()

# ---------
# BUILD CLI
# ---------
if(BUILD_CLI)
  message(STATUS "NOTICE: Configuring CLI build.")
  
  # The variable UNIX will also return true if cmake was run via CygWin
  if(NOT UNIX OR CYGWIN)
    message(STATUS "")
    message(STATUS "WARNING: Non-UNIX OS detected:")
    message(STATUS "WARNING: * The command-line interface is only intented to be used on UNIX-systems. Use the CLI at your on risk.")
    message(STATUS "")
  endif()
  
  # Locate libraries
  find_package(Readline REQUIRED)
  
  # Include headers
  include_directories(${READLINE_INCLUDE_DIRS})
  
  # Compile the CLI
  # All paths must start with "cli/"
  add_executable(cli
    cli/main.cpp
  )
  target_link_libraries(cli ${READLINE_LIBRARIES})
endif()
