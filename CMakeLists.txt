#------------------------------------------------------------------------------
# Copyright 2013-2015 Sebastian Niemann <niemann@sra.uni-hannover.de>.
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
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Mantella, C++ optimisation framework
project(Mantella CXX C)

set(MANTELLA_MAJOR 0)
set(MANTELLA_MINOR 9)
set(MANTELLA_PATCH 0)
set(MANTELLA_VERSION "${MANTELLA_MAJOR}.${MANTELLA_MINOR}.${MANTELLA_PATCH}")

# Add additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Development build options
option(BUILD_TESTS "Build tests" OFF)
option(RUN_TESTS "Run (and build) tests" OFF)
option(MEASURE_CODE_COVERAGE "Measure code coverage" OFF)

if(RUN_TESTS)
  set(BUILD_TESTS ON)
endif()

mark_as_advanced(BUILD_TESTS RUN_TESTS MEASURE_CODE_COVERAGE)

if(WIN32)
  message(STATUS "")
  message(STATUS "INSTALLATION INSTRUCTION: Mantella is a header-only C++11 library, it is only required to copy the include/ directory into a folder searched by the #include preprocessor directive.")
  message(STATUS "")
else()
  # ---------------
  # PREPARE LIBRARY
  # ---------------
  message(STATUS "NOTICE: Checking prerequirements for the Mantella library (version: ${MANTELLA_VERSION}).")
    
  # Locate third party libraries and headers
  find_package(Armadillo 4.650.0 REQUIRED) # Armadillo C++

  message(STATUS "")
  message(STATUS "INSTALLATION INSTRUCTION: Mantella is a header-only C++11 library, it is only required to copy the include/ directory into a folder searched by the #include preprocessor directive (or use *sudo make install* as a shortcut).")
  message(STATUS "")

  # ----------
  # INSTALLING
  # ----------

  if(NOT INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "include")
  endif()

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)
endif()

# -----------
# BUILD TESTS
# -----------

if(BUILD_TESTS)
  message(STATUS "NOTICE: Configuring tests")

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

  if(MEASURE_CODE_COVERAGE)
    # Use debug mode, no optimisation, measure coverage
    message(STATUS "NOTICE: Measure code coverage.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
  endif()

  # Locate third party libraries and headers
  if(WIN32)
    find_library(ARMADILLO_LIBRARIES openblas HINTS "C:/Program\ Files/OpenBLAS/lib")
    find_path(ARMADILLO_INCLUDE_DIR armadillo HINTS "C:/Program\ Files/Armadillo/include")
    find_path(CATCH_INCLUDE_DIRS catch.hpp HINTS "C:/Program\ Files/Catch")
  else()
    find_package(Armadillo 4.650.0 REQUIRED) # Armadillo C++
    find_package(Catch REQUIRED) # Catch (header only)
  endif()
  
  # Include Mantella
  include_directories(${PROJECT_SOURCE_DIR}/include)
  
  # Include third party headers
  include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
  include_directories(${CATCH_INCLUDE_DIRS}) # Catch

  # Compile the optimisation library tests
  # All paths must start with "test/"
  add_executable(libraryTest
    test/main.cpp
    test/helper/testRng.cpp
    test/helper/testFilesystem.cpp
    test/helper/testGeometry.cpp
    test/helper/testPrintable.cpp
    test/helper/testQuasiRandomSequence.cpp
    test/helper/testRandom.cpp
    test/helper/testSetTheory.cpp
    test/helper/testString.cpp
    test/helper/testUnorderedContainer.cpp
    test/testDistanceFunction.cpp
    test/distanceFunction/testHammingDistance.cpp
    test/distanceFunction/testManhattanDistance.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testAttractiveSectorFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testBentCigarFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testBuecheRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testDifferentPowersFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testDiscusFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testKatsuuraFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testLinearSlope.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testRastriginFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testRosenbrockFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testSchaffersF7Function.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testSchwefelFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testSharpRidgeFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testSphereFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark/testWeierstrassFunction.cpp
  )

  # Link against third party libraries
  target_link_libraries(libraryTest ${ARMADILLO_LIBRARIES}) # Armadillo C++

  if(RUN_TESTS)
    message(STATUS "NOTICE: Automatically running all tests after building them.")

    add_custom_command(TARGET libraryTest POST_BUILD COMMAND libraryTest ${PROJECT_SOURCE_DIR}/test)
  endif()
endif()

# ---------------------
# Qt Creator workaround
# ---------------------
add_custom_target(QtWorkaround SOURCES
  # Library
  include/mantella
  include/mantella_bits/helper/assert.hpp
  include/mantella_bits/helper/rng.hpp
  include/mantella_bits/helper/filesystem.hpp
  include/mantella_bits/helper/geometry.hpp
  include/mantella_bits/helper/printable.hpp
  include/mantella_bits/helper/quasiRandomSequence.hpp
  include/mantella_bits/helper/random.hpp
  include/mantella_bits/helper/regression.hpp
  include/mantella_bits/helper/setTheory.hpp
  include/mantella_bits/helper/string.hpp
  include/mantella_bits/helper/trajectory.hpp
  include/mantella_bits/helper/unorderedContainer.hpp
  include/mantella_bits/distanceFunction.hpp
  include/mantella_bits/distanceFunction/hammingDistance.hpp
  include/mantella_bits/distanceFunction/manhattanDistance.hpp
  include/mantella_bits/optimisationProblem.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/bentCigarFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/buecheRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/differentPowersFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/discusFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian21hiPeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/katsuuraFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/linearSlope.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/schaffersF7Function.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/schaffersF7FunctionIllConditioned.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/schwefelFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/sharpRidgeFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/sphereFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/stepEllipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/weierstrassFunction.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine3PRPR.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine3PRRR.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine3PRUS.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine3PUPS.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine4PRUS.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine6PRUS.hpp
  include/mantella_bits/optimisationProblem/robotic/model/parallelKinematicMachine6PUPS.hpp
  include/mantella_bits/optimisationProblem/robotic/model/multiLevelStewartPlatform.hpp
  include/mantella_bits/optimisationProblem/robotic/poseAccuracy.hpp
  include/mantella_bits/optimisationAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/gridSearch.hpp
  include/mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/randomSearch.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hillClimbing.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hookeJeevesAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/simulatedAnnealing.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/standardParticleSwarmOptimisation2011.hpp
  include/mantella_bits/propertyAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/fitnessDistanceCorrelationAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/fitnessCloudCorrelationAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/lipschitzContinuityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/alphaHoelderContinuityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/ordinaryLeastSquaresLinearFunctionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/ordinaryLeastSquaresQuadraticFunctionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/additiveSeparabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/multiplicativeSeparabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/linearProportionalityAnalysis.hpp
  # Tests
  test/helper.hpp
)
