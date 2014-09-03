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
cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Hint-based Online Programming (HOP)
project(HOP CXX)

# Add additional cmake modules (currently unused)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ext/cmake)

# Provide custom build management options
option(BUILD_LIBRARY "Build library (includes tests)" ON)
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

# Include thrid-party headers
include_directories(${PROJECT_SOURCE_DIR}/ext)

# -------------
# BUILD LIBRARY
# -------------
if(BUILD_LIBRARY)
  message(STATUS "NOTICE: Configuring Hint-based Online Programming (HOP) library build.")

  # Locate libraries and headers
  find_package(Armadillo REQUIRED) # Armadillo C++
  find_package(Boost COMPONENTS system filesystem REQUIRED) # Boost (system, filesystem)

  # Include headers
  include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++
  include_directories(${BOOST_INCLUDE_DIRS}) # Boost

  # Compile the optimisation library
  # All paths must start with "src/"
  add_library(hop SHARED
    src/optimisationProblem.cpp
    src/optimisationProblem/benchmarkProblem.cpp
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
    src/optimisationAlgorithm.cpp
    src/evaluation/monitor.cpp
    src/evaluation/selector.cpp
    src/evaluation/aggregator.cpp
    src/evaluation/runable.cpp
    src/evaluation/experiment.cpp
    src/evaluation/analysis.cpp
    src/evaluation/analysis/convergenceRate.cpp
    src/evaluation/analysis/fitnessFunctionGrid.cpp
    src/evaluation/analysis/performanceProfile.cpp
    src/helper/cereal.cpp
    src/helper/random.cpp
  )

  # Link against libraries
  target_link_libraries(hop ${ARMADILLO_LIBRARIES}) # Armadillo C++
  target_link_libraries(hop ${Boost_LIBRARIES}) # Boost

  # -----------
  # BUILD TESTS
  # -----------

  # Include headers
  include_directories(${PROJECT_SOURCE_DIR}/test) # HOP tests

  # Compile the optimisation library tests
  # All paths must start with "test/"
  add_executable(test
    test/main.cpp
  )

  # Add library directories
  link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) # HOP

  # Link against libraries
  target_link_libraries(test hop) # HOP
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

  # Include headers
  include_directories(${PROJECT_SOURCE_DIR}/cli) # CLI
  
  # Compile the CLI
  # All paths must start with "cli/"
  add_executable(cli
    cli/main.cpp
    cli/config.cpp
  )

  # Add library directories
  link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) # HOP

  # Link against libraries
  target_link_libraries(cli hop) # HOP
endif()

# ---------------------
# Qt Creator workaround
# ---------------------
add_custom_target(QtWorkaround SOURCES
  include/hop_bits/optimisationProblem.hpp
  include/hop_bits/optimisationProblem/benchmarkProblem.hpp
  include/hop_bits/optimisationProblem/benchmark/attractiveSectorFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bentCigarFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/buecheRastriginFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/hop_bits/optimisationProblem/benchmark/differentPowersFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/discusFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/ellipsoidalFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/ellipsoidalFunctionRotated.hpp
  include/hop_bits/optimisationProblem/benchmark/gallaghersGaussian101mePeaksFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/gallaghersGaussian21hiPeaksFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/katsuuraFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/linearSlope.hpp
  include/hop_bits/optimisationProblem/benchmark/lunacekBiRastriginFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/rastriginFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/rastriginFunctionRotated.hpp
  include/hop_bits/optimisationProblem/benchmark/rosenbrockFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/rosenbrockFunctionRotated.hpp
  include/hop_bits/optimisationProblem/benchmark/schaffersF7Function.hpp
  include/hop_bits/optimisationProblem/benchmark/schaffersF7FunctionIllConditioned.hpp
  include/hop_bits/optimisationProblem/benchmark/schwefelFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/sharpRidgeFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/sphereFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/stepEllipsoidalFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/weierstrassFunction.hpp
  include/hop_bits/optimisationAlgorithm.hpp
  include/hop_bits/evaluation/monitor.hpp
  include/hop_bits/evaluation/selector.hpp
  include/hop_bits/evaluation/aggregator.hpp
  include/hop_bits/evaluation/runable.hpp
  include/hop_bits/evaluation/experiment.hpp
  include/hop_bits/evaluation/analysis.hpp
  include/hop_bits/evaluation/analysis/convergenceRate.hpp
  include/hop_bits/evaluation/analysis/fitnessFunctionGrid.hpp
  include/hop_bits/evaluation/analysis/performanceProfile.hpp
  include/hop_bits/helper/cereal.hpp
  include/hop_bits/helper/random.hpp
)
