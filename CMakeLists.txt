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
#   Romeo Shuka - Developer
#   Wolff Bock von Wülfingen - Developer
#   Daniel Kiechle - Developer / Software tester
#   Oskar Pusz - Developer
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Hint-based Online Programming (HOP)
project(HOP C CXX)

# Add additional cmake modules (currently unused)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ext/cmake)

# Provide custom build management options
option(BUILD_LIBRARY "Build library (includes tests)" ON)
option(BUILD_GUI "Build GUI" OFF)
option(BUILD_APPS "Build APPS" OFF)
option(BUILD_ALL "Build all" OFF)

if(BUILD_ALL)
  set(BUILD_LIBRARY ON)
  set(BUILD_GUI ON)
  set(BUILD_APPS ON)
endif()

# OSX
if(APPLE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif()
endif()

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# Enable C++11 features
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

# Include HOP headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# -------------
# BUILD LIBRARY
# -------------
if(BUILD_LIBRARY)
  message(STATUS "NOTICE: Configuring Hint-based Online Programming (HOP) library build.")
  
  # Locate third party libraries and headers
  find_package(Armadillo REQUIRED) # Armadillo C++
  find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED) # Boost (system, filesystem)
  find_package(MPI REQUIRED) # MPI

  # Include third party headers
  include_directories(${PROJECT_SOURCE_DIR}/ext) # Catch and Cereal
  include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++
  include_directories(${Boost_INCLUDE_DIRS}) # Boost
  include_directories(${MPI_C_INCLUDE_PATH}) # MPI C-bindings

  # Compile the optimisation library
  # All paths must start with "src/"
  add_library(hop SHARED
    src/optimisationAlgorithm.cpp
    src/optimisationAlgorithm/hillClimbing.cpp
    src/optimisationAlgorithm/hookeJeevesAlgorithm.cpp
    src/optimisationAlgorithm/randomSearch.cpp
   # src/optimisationAlgorithm/roleBasedImitationAlgorithm.cpp
    src/optimisationAlgorithm/standardParticleSwarmOptimisation2011.cpp
    src/optimisationAlgorithm/parallelOptimisationAlgorithm.cpp
    src/optimisationAlgorithm/parallel/standardParticleSwarmOptimisation2011.cpp
    src/optimisationProblem.cpp
    src/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.cpp
    src/optimisationProblem/benchmark/bbob2013/attractiveSectorFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/bentCigarFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/buecheRastriginFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/benchmark/bbob2013/differentPowersFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/discusFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/ellipsoidalFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/benchmark/bbob2013/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/katsuuraFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/linearSlope.cpp
    src/optimisationProblem/benchmark/bbob2013/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/rastriginFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/rastriginFunctionRotated.cpp
    src/optimisationProblem/benchmark/bbob2013/rosenbrockFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/benchmark/bbob2013/schaffersF7Function.cpp
    src/optimisationProblem/benchmark/bbob2013/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/benchmark/bbob2013/schwefelFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/sharpRidgeFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/sphereFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/stepEllipsoidalFunction.cpp
    src/optimisationProblem/benchmark/bbob2013/weierstrassFunction.cpp
    src/evaluation.cpp
    src/evaluation/monitor/aggregator.cpp
    src/evaluation/monitor/selector.cpp
    src/evaluation/monitor.cpp
    src/helper/cereal.cpp
    src/helper/printable.cpp
    src/helper/random.cpp
    src/helper/runable.cpp
    src/helper/to_string.cpp
  )

  # Link against third party libraries
  target_link_libraries(hop ${ARMADILLO_LIBRARIES}) # Armadillo C++
  target_link_libraries(hop ${Boost_LIBRARIES}) # Boost
  target_link_libraries(hop ${MPI_C_LIBRARIES}) # MPI C-bindings

  # -----------
  # BUILD TESTS
  # -----------

  # Include headers
  include_directories(${PROJECT_SOURCE_DIR}/test) # HOP tests

  # Compile the optimisation library tests
  # All paths must start with "test/"
  add_executable(libTest
    test/main.cpp
  )

  # Add library directories
  link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) # HOP

  # Link against libraries
  target_link_libraries(libTest hop) # HOP
  
  add_custom_command(TARGET libTest POST_BUILD COMMAND libTest ${PROJECT_SOURCE_DIR}/test)
endif()

# ---------
# BUILD GUI
# ---------
if(BUILD_GUI)
  message(STATUS "")
  message(STATUS "ERROR: Building the GUI via cmake is not supported. Use qmake instead.")
  message(STATUS "")
endif()

# ----------
# BUILD APPS
# ----------
if(BUILD_APPS)
  message(STATUS "NOTICE: Configuring applications build.")

  # Add applications build script
  add_subdirectory("./apps") 
endif()

# ---------------------
# Qt Creator workaround
# ---------------------
add_custom_target(QtWorkaround SOURCES
  include/hop
  include/hop_bits/optimisationAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/hillClimbing.hpp
  include/hop_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/randomSearch.hpp
  include/hop_bits/optimisationAlgorithm/roleBasedImitationAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/standardParticleSwarmOptimisation2011.hpp
  include/hop_bits/optimisationAlgorithm/parallelOptimisationAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/parallel/standardParticleSwarmOptimisation2011.hpp
  include/hop_bits/optimisationProblem.hpp
  include/hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/attractiveSectorFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/bentCigarFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/buecheRastriginFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/differentPowersFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/discusFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/ellipsoidalFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/ellipsoidalFunctionRotated.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/gallaghersGaussian101mePeaksFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/gallaghersGaussian21hiPeaksFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/katsuuraFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/linearSlope.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/lunacekBiRastriginFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/rastriginFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/rastriginFunctionRotated.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/rosenbrockFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/rosenbrockFunctionRotated.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/schaffersF7Function.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/schaffersF7FunctionIllConditioned.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/schwefelFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/sharpRidgeFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/sphereFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/stepEllipsoidalFunction.hpp
  include/hop_bits/optimisationProblem/benchmark/bbob2013/weierstrassFunction.hpp
  include/hop_bits/evaluation.hpp
  include/hop_bits/evaluation/monitor/aggregator.hpp
  include/hop_bits/evaluation/monitor/selector.hpp
  include/hop_bits/evaluation/monitor.hpp
  include/hop_bits/helper/cereal.hpp
  include/hop_bits/helper/printable.hpp
  include/hop_bits/helper/random.hpp
  include/hop_bits/helper/runable.hpp
  include/hop_bits/helper/to_string.hpp
  test/optimisationProblem/benchmark/testBlackBoxOptimisationBenchmark2013.hpp
)
