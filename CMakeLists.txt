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
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

# Mantella, C++ optimisation framework
project(Mantella CXX C)

set(MANTELLA_MAJOR 0)
set(MANTELLA_MINOR 9)
set(MANTELLA_PATCH 0)
set(MANTELLA_VERSION "${MANTELLA_MAJOR}.${MANTELLA_MINOR}.${MANTELLA_PATCH}")

# Add additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ext/cmake)

# Provide custom build management options
option(BUILD_LIBRARY "Build library" ON)
option(BUILD_TEST "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_ALL "Build all" OFF)

if(BUILD_ALL)
  set(BUILD_LIBRARY ON)
  set(BUILD_TEST ON)
  set(BUILD_EXAMPLES ON)
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
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
endif()

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Display all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Use native machine features (SSE3, SSE4, AVX, ...), if possible and compiled with Armadillo
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

# Include our own headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# -------------
# BUILD LIBRARY
# -------------
if(BUILD_LIBRARY)
  message(STATUS "NOTICE: Configuring Hint-based Online Programming (Mantella) library (version: ${MANTELLA_VERSION}).")
  
  # Locate third party libraries and headers
  find_package(Armadillo 4.600.0 REQUIRED) # Armadillo C++
  find_package(Cereal REQUIRED) # Cereal (header only)
  find_package(MPI 3.0.0) # MPI

  # Include third party headers
  include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
  include_directories(${CEREAL_INCLUDE_DIRS}) # Cereal

  # Source files without MPI usage (serial algorithms only)
  # All paths must start with "src/"
  set(MANTELLA_LIBRARY_SOURCES
    src/distanceFunction.cpp
    src/distanceFunction/euclideanDistance.cpp
    src/optimisationProblem.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/attractiveSectorFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/bentCigarFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/buecheRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/differentPowersFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/discusFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/ellipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/katsuuraFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/linearSlope.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/rastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/rastriginFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/rosenbrockFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/schaffersF7Function.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/schwefelFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/sharpRidgeFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/sphereFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/stepEllipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2009/weierstrassFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/attractiveSectorFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/bentCigarFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/buecheRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/differentPowersFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/discusFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/ellipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/katsuuraFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/linearSlope.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/rastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/rastriginFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/rosenbrockFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/schaffersF7Function.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/schwefelFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/sharpRidgeFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/sphereFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/stepEllipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2010/weierstrassFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/attractiveSectorFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/bentCigarFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/buecheRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/differentPowersFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/discusFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/ellipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/katsuuraFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/linearSlope.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/rastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/rastriginFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/rosenbrockFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/schaffersF7Function.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/schwefelFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/sharpRidgeFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/sphereFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/stepEllipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2012/weierstrassFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/attractiveSectorFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/bentCigarFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/buecheRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/differentPowersFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/discusFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/ellipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/katsuuraFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/linearSlope.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/rastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/rastriginFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/rosenbrockFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/schaffersF7Function.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/schwefelFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/sharpRidgeFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/sphereFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/stepEllipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2013/weierstrassFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/attractiveSectorFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/bentCigarFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/buecheRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/compositeGriewankRosenbrockFunctionF8F2.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/differentPowersFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/discusFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/ellipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/ellipsoidalFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/gallaghersGaussian101mePeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/gallaghersGaussian21hiPeaksFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/katsuuraFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/linearSlope.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/lunacekBiRastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/rastriginFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/rastriginFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/rosenbrockFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/rosenbrockFunctionRotated.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/schaffersF7Function.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/schaffersF7FunctionIllConditioned.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/schwefelFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/sharpRidgeFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/sphereFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/stepEllipsoidalFunction.cpp
    src/optimisationProblem/blackBoxOptimisationBenchmark2015/weierstrassFunction.cpp
    src/optimisationProblem/kinematicMachine.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prpr.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prrr.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prus.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_3pups.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_4prus.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_6prus.cpp
    src/optimisationProblem/kinematicMachine/parallelKinematicMachine_6pups.cpp
    src/optimisationProblem/kinematicMachine/multiLevelStewartPlatform.cpp
    src/optimisationProblem/nkModel.cpp
    src/optimisationAlgorithm.cpp
    src/optimisationAlgorithm/samplingBasedAlgorithm.cpp
    src/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.cpp
    src/optimisationAlgorithm/samplingBasedAlgorithm/multiResolutionGridSearch.cpp
    src/optimisationAlgorithm/samplingBasedAlgorithm/randomSearch.cpp
    src/optimisationAlgorithm/trajectoryBasedAlgorithm.cpp
    src/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.cpp
    src/optimisationAlgorithm/trajectoryBasedAlgorithm/hookeJeevesAlgorithm.cpp
    src/optimisationAlgorithm/trajectoryBasedAlgorithm/simulatedAnnealing.cpp
    src/optimisationAlgorithm/populationBasedAlgorithm.cpp
    src/optimisationAlgorithm/populationBasedAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.cpp
    src/optimisationAlgorithm/populationBasedAlgorithm/roleBasedImitationAlgorithm.cpp
    src/optimisationAlgorithm/populationBasedAlgorithm/standardParticleSwarmOptimisation2011.cpp
    src/propertiesAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis/fitnessDistanceCorrelationAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis/directLipschitzContinuityAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis/linearOrdinaryLeastSquares.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.cpp
    src/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis/quadraticOrdinaryLeastSquares.cpp
    src/propertiesAnalysis/activePropertiesAnalysis.cpp
    src/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.cpp
    src/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.cpp
    src/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis/directAdditiveSeparabilityAnalysis.cpp
    src/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis.cpp
    src/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.cpp
    src/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis/directLinearProportionalityAnalysis.cpp
    src/propertiesEstimation.cpp
    src/propertiesEstimation/maximumLikelihoodEstimation.cpp
    src/helper/cereal.cpp
    src/helper/combinatorics.cpp
    src/helper/geometry.cpp
    src/helper/printable.cpp
    src/helper/quasiRandomSequence.cpp
    src/helper/random.cpp
    src/helper/rng.cpp
    src/helper/string.cpp
    src/helper/unordered_container.cpp
  )

  if(MPI_C_FOUND)
    message(STATUS "NOTICE: Compile with MPI support (adding parallel algorithms).")

    include_directories(${MPI_C_INCLUDE_PATH}) # MPI C-bindings

    # Compile the optimisation library with MPI support (parallel algorithms)
    add_library(mantella SHARED
      ${MANTELLA_LIBRARY_SOURCES}
      src/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm.cpp
      src/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm/parallelStandardParticleSwarmOptimisation2011.cpp
    )

    # Link against third party libraries
    target_link_libraries(mantella ${MPI_C_LIBRARIES}) # MPI C-bindings
  else()
    message(STATUS "NOTICE: Compile without MPI support (serial algorithms only).")

    # Compile the optimisation library without MPI support (serial algorithms only)
    add_library(mantella SHARED
      ${MANTELLA_LIBRARY_SOURCES}
    )
  endif()

  # Link against third party libraries
  target_link_libraries(mantella ${ARMADILLO_LIBRARIES}) # Armadillo C++

  # -----------
  # BUILD TESTS
  # -----------

  if(BUILD_TEST)
      message(STATUS "NOTICE: Configuring tests")

      # Locate third party libraries and headers
      find_package(Armadillo 4.600.0 REQUIRED) # Armadillo C++
      find_package(Catch REQUIRED) # Catch (header only)
      find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED) # Boost (system, filesystem)

      # Include third party headers
      include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
      include_directories(${CATCH_INCLUDE_DIRS}) # Catch
      include_directories(${Boost_INCLUDE_DIRS}) # Boost

      # Include headers
      include_directories(${PROJECT_SOURCE_DIR}/test) # MANTELLA tests

      # Compile the optimisation library tests
      # All paths must start with "test/"
      add_executable(libTest
        test/main.cpp
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
	test/optimisationAlgorithm/trajectoryBasedAlgorithm/testHillClimbing.cpp
        test/helper/testGeometry.cpp
        test/helper/testRandom.cpp
        test/helper/testString.cpp
      )

      # Add MANTELLA library directory
      link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) # MANTELLA

      # Link against MANTELLA library
      target_link_libraries(libTest mantella) # MANTELLA

      # Link against third party libraries
      target_link_libraries(libTest ${ARMADILLO_LIBRARIES}) # Armadillo C++
      target_link_libraries(libTest ${Boost_LIBRARIES}) # Boost

      add_custom_command(TARGET libTest POST_BUILD COMMAND libTest ${PROJECT_SOURCE_DIR}/test)
  endif()

  # ----------
  # INSTALLING
  # ----------

  if(NOT INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "include")
  endif()

  if(NOT INSTALL_LIB_DIR)
    set(INSTALL_LIB_DIR "lib")
  endif()

  set_property(TARGET mantella PROPERTY VERSION ${MANTELLA_VERSION})
  set_property(TARGET mantella PROPERTY SOVERSION ${MANTELLA_MAJOR})

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)

  install(TARGETS mantella
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR})

#  include(CMakePackageConfigHelpers)
#  write_basic_package_version_file("${PROJECT_SOURCE_DIR}/MANTELLAConfigVersion.cmake" VERSION ${MANTELLA_VERSION} COMPATIBILITY AnyNewerVersion)
endif()

# --------------
# BUILD EXAMPLES
# -------------
if(BUILD_EXAMPLES)
  message(STATUS "NOTICE: Configuring applications.")

  # Add applications build script
  add_subdirectory("./examples")
endif()

# ---------------------
# Qt Creator workaround
# ---------------------
add_custom_target(QtWorkaround SOURCES
  include/mantella
  include/mantella_bits/distanceFunction.hpp
  include/mantella_bits/distanceFunction/euclideanDistance.hpp
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
  include/mantella_bits/optimisationProblem/kinematicMachine.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prpr.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prrr.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prus.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3pups.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_4prus.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_6prus.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_6pups.hpp
  include/mantella_bits/optimisationProblem/kinematicMachine/multiLevelStewartPlatform.hpp
  include/mantella_bits/optimisationProblem/nkModel.hpp
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
  include/mantella_bits/propertiesAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis/fitnessDistanceCorrelationAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis/directLipschitzContinuityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis/linearOrdinaryLeastSquares.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis/quadraticOrdinaryLeastSquares.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis/directAdditiveSeparabilityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp
  include/mantella_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis/directLinearProportionalityAnalysis.hpp
  include/mantella_bits/propertiesEstimation.hpp
  include/mantella_bits/propertiesEstimation/maximumLikelihoodEstimation.hpp
  include/mantella_bits/helper/cereal.hpp
  include/mantella_bits/helper/combinatorics.hpp
  include/mantella_bits/helper/geometry.hpp
  include/mantella_bits/helper/printable.hpp
  include/mantella_bits/helper/quasiRandomSequence.hpp
  include/mantella_bits/helper/random.hpp
  include/mantella_bits/helper/rng.hpp
  include/mantella_bits/helper/string.hpp
  include/mantella_bits/helper/unordered_container.hpp
)
