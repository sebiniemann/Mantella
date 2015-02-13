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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ext/cmake)

# Build options used only during development
option(BUILD_TESTS "Build tests" OFF)
option(RUN_TESTS "Run (and build) tests" OFF)
if(RUN_TESTS)
  set(BUILD_TESTS ON)
endif()
mark_as_advanced(BUILD_TESTS RUN_TESTS)

option(MEASURE_CODE_COVERAGE "Measure code coverage" OFF)
mark_as_advanced(MEASURE_CODE_COVERAGE)

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

# Include our own headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# ---------------
# PREPARE LIBRARY
# ---------------
message(STATUS "NOTICE: Checking prerequirements for the Mantella library (version: ${MANTELLA_VERSION}).")
  
# Locate third party libraries and headers
find_package(Armadillo 4.600.0 REQUIRED) # Armadillo C++
message(STATUS "NOTICE: Found Armadillo.")

find_package(Cereal) # Cereal (header only)
find_package(MPI 3.0.0) # MPI

if(MPI_C_FOUND AND CEREAL_FOUND)
  message(STATUS "NOTICE: Found MPI and Cereal (adding cluster computation support).")
else()
  message(STATUS "NOTICE: Could not find MPI and/or Cereal (excluding cluser computation support).")
endif()


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

# -----------
# BUILD TESTS
# -----------

if(BUILD_TESTS)
  message(STATUS "NOTICE: Configuring tests")

  # Locate third party libraries and headers
  find_package(Armadillo 4.600.0 REQUIRED) # Armadillo C++
  find_package(Catch REQUIRED) # Catch (header only)

  # Include third party headers
  include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
  include_directories(${CATCH_INCLUDE_DIRS}) # Catch

  # Include headers
  include_directories(${PROJECT_SOURCE_DIR}/test) # MANTELLA tests

  set(MANTELLA_TEST_PARALLEL
#    test/helper/testCereal.cpp
  )

  if(NOT MPI_C_FOUND OR NOT CEREAL_FOUND)
    set(MANTELLA_TEST_PARALLEL "")
    message(STATUS "NOTICE: Found MPI and Cereal (adding cluster computation test cases).")
  else()
    message(STATUS "NOTICE: Could not find MPI and/or Cereal (excluding cluser computation test cases).")
  endif()

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
    test/distanceFunction/testEuclideanDistance.cpp
    test/distanceFunction/testHammingDistance.cpp
    test/distanceFunction/testInfinityNorm.cpp
    test/distanceFunction/testManhattanDistance.cpp
    test/testOptimisationProblem.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testAttractiveSectorFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testBentCigarFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testBuecheRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testDifferentPowersFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testDiscusFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testKatsuuraFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testLinearSlope.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testRastriginFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testRosenbrockFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testSchaffersF7Function.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testSchwefelFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testSharpRidgeFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testSphereFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2009/testWeierstrassFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testAttractiveSectorFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testBentCigarFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testBuecheRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testDifferentPowersFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testDiscusFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testKatsuuraFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testLinearSlope.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testRastriginFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testRosenbrockFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testSchaffersF7Function.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testSchwefelFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testSharpRidgeFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testSphereFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2010/testWeierstrassFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testAttractiveSectorFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testBentCigarFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testBuecheRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testDifferentPowersFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testDiscusFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testKatsuuraFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testLinearSlope.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testRastriginFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testRosenbrockFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testSchaffersF7Function.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testSchwefelFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testSharpRidgeFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testSphereFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2012/testWeierstrassFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testAttractiveSectorFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testBentCigarFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testBuecheRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testDifferentPowersFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testDiscusFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testKatsuuraFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testLinearSlope.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testRastriginFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testRosenbrockFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testSchaffersF7Function.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testSchwefelFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testSharpRidgeFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testSphereFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2013/testWeierstrassFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testAttractiveSectorFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testBentCigarFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testBuecheRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testDifferentPowersFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testDiscusFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testKatsuuraFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testLinearSlope.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testRastriginFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testRastriginFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testRosenbrockFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testSchaffersF7Function.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testSchwefelFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testSharpRidgeFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testSphereFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/blackBoxOptimisationBenchmark2015/testWeierstrassFunction.cpp
    test/testOptimisationAlgorithm.cpp
    test/testPropertyAnalysis.cpp
    test/testPropertiesEstimation.cpp
#        test/optimisationAlgorithm/trajectoryBasedAlgorithm/testHillClimbing.cpp
#        test/optimisationAlgorithm/trajectoryBasedAlgorithm/testSimulatedAnnealing.cpp
#        test/optimisationAlgorithm/trajectoryBasedAlgorithm/testHookeJeeves.cpp
    ${MANTELLA_TEST_PARALLEL}
  )

  # Link against third party libraries
  target_link_libraries(libraryTest ${ARMADILLO_LIBRARIES}) # Armadillo C++

  if(RUN_TESTS)
    message(STATUS "NOTICE: Automatically runs all tests after building them.")

    add_custom_command(TARGET libTest POST_BUILD COMMAND libTest ${PROJECT_SOURCE_DIR}/test)
  endif()
endif()

# ---------------------
# Qt Creator workaround
# ---------------------
add_custom_target(QtWorkaround SOURCES
  # Library
  include/mantella
  include/mantella_bits/helper/rng.hpp
  include/mantella_bits/helper/cereal.hpp
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
  include/mantella_bits/distanceFunction/euclideanDistance.hpp
  include/mantella_bits/distanceFunction/hammingDistance.hpp
  include/mantella_bits/distanceFunction/infinityNorm.hpp
  include/mantella_bits/distanceFunction/manhattanDistance.hpp
  include/mantella_bits/optimisationProblem.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/attractiveSectorFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/bentCigarFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/buecheRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/differentPowersFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/discusFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/ellipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/ellipsoidalFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/gallaghersGaussian101mePeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/gallaghersGaussian21hiPeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/katsuuraFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/linearSlope.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/lunacekBiRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/rastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/rastriginFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/rosenbrockFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/rosenbrockFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/schaffersF7Function.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/schaffersF7FunctionIllConditioned.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/schwefelFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/sharpRidgeFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/sphereFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/stepEllipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/weierstrassFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/attractiveSectorFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/bentCigarFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/buecheRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/differentPowersFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/discusFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/ellipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/ellipsoidalFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/gallaghersGaussian101mePeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/gallaghersGaussian21hiPeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/katsuuraFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/linearSlope.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/lunacekBiRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/rastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/rastriginFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/rosenbrockFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/rosenbrockFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/schaffersF7Function.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/schaffersF7FunctionIllConditioned.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/schwefelFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/sharpRidgeFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/sphereFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/stepEllipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/weierstrassFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/attractiveSectorFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/bentCigarFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/buecheRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/differentPowersFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/discusFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/ellipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/ellipsoidalFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/gallaghersGaussian101mePeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/gallaghersGaussian21hiPeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/katsuuraFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/linearSlope.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/lunacekBiRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/rastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/rastriginFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/rosenbrockFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/rosenbrockFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/schaffersF7Function.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/schaffersF7FunctionIllConditioned.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/schwefelFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/sharpRidgeFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/sphereFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/stepEllipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/weierstrassFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/attractiveSectorFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/bentCigarFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/buecheRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/differentPowersFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/discusFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/ellipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/ellipsoidalFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/gallaghersGaussian101mePeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/gallaghersGaussian21hiPeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/katsuuraFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/linearSlope.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/lunacekBiRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rastriginFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rosenbrockFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rosenbrockFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/schaffersF7Function.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/schaffersF7FunctionIllConditioned.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/schwefelFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/sharpRidgeFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/sphereFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/stepEllipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/weierstrassFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/attractiveSectorFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/bentCigarFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/buecheRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/differentPowersFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/discusFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/ellipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/ellipsoidalFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/gallaghersGaussian101mePeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/gallaghersGaussian21hiPeaksFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/katsuuraFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/linearSlope.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/lunacekBiRastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/rastriginFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/rastriginFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/rosenbrockFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/rosenbrockFunctionRotated.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/schaffersF7Function.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/schaffersF7FunctionIllConditioned.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/schwefelFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/sharpRidgeFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/sphereFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/stepEllipsoidalFunction.hpp
  include/mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/weierstrassFunction.hpp
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
  include/mantella_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp
  include/mantella_bits/optimisationAlgorithm/samplingBasedAlgorithm/multiResolutionGridSearch.hpp
  include/mantella_bits/optimisationAlgorithm/samplingBasedAlgorithm/randomSearch.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hookeJeevesAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/simulatedAnnealing.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedAlgorithm/roleBasedImitationAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedAlgorithm/standardParticleSwarmOptimisation2011.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm.hpp
  include/mantella_bits/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm/parallelStandardParticleSwarmOptimisation2011.hpp
  include/mantella_bits/property.hpp
  include/mantella_bits/property/correlation.hpp
  include/mantella_bits/property/lipschitzContinuity.hpp
  include/mantella_bits/property/linearFunctionModel.hpp
  include/mantella_bits/property/quadraticFunctionModel.hpp
  include/mantella_bits/property/additiveSeparability.hpp
  include/mantella_bits/property/multiplicativeSeparability.hpp
  include/mantella_bits/property/linearProportionality.hpp
  include/mantella_bits/propertyAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/correlationAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/correlationAnalysis/fitnessDistanceCorrelationAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/continuityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/continuityAnalysis/lipschitzContinuityAnalysis/directLipschitzContinuityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/functionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/functionModelAnalysis/linearFunctionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/functionModelAnalysis/linearFunctionModelAnalysis/ordinaryLeastSquaresLinearFunctionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/functionModelAnalysis/quadraticFunctionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/passivePropertyAnalysis/functionModelAnalysis/quadraticFunctionModelAnalysis/ordinaryLeastSquaresQuadraticFunctionModelAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/separabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis/directAdditiveSeparabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/separabilityAnalysis/multiplicativeSeparabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/separabilityAnalysis/multiplicativeSeparabilityAnalysis/directMultiplicativeSeparabilityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/proportionalityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp
  include/mantella_bits/propertyAnalysis/activePropertyAnalysis/proportionalityAnalysis/linearProportionalityAnalysis/directLinearProportionalityAnalysis.hpp
  include/mantella_bits/propertiesEstimation.hpp
  # Tests
  test/helper.hpp
)
