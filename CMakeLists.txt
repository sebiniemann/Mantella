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
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

# Hint-based Online Programming (HOP)
project(HOP CXX C)

set(HOP_MAJOR 0)
set(HOP_MINOR 9)
set(HOP_PATCH 0)
set(HOP_VERSION "${HOP_MAJOR}.${HOP_MINOR}.${HOP_PATCH}")

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
  message(STATUS "NOTICE: Configuring Hint-based Online Programming (HOP) library (version: ${HOP_VERSION}).")
  
  # Locate third party libraries and headers
  find_package(Armadillo 4.600.0 REQUIRED) # Armadillo C++
  find_package(Cereal REQUIRED) # Cereal (header only)
  find_package(MPI 3.0.0) # MPI

  # Include third party headers
  include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
  include_directories(${CEREAL_INCLUDE_DIRS}) # Cereal

  # Source files without MPI usage (serial algorithms only)
  # All paths must start with "src/"
  set(HOP_LIBRARY_SOURCES
    src/distanceFunction.cpp
    src/distanceFunction/euclideanDistance.cpp
    src/optimisationProblem.cpp
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
    src/helper/unordered_map.cpp
  )

  if(MPI_C_FOUND)
    message(STATUS "NOTICE: Compile with MPI support (adding parallel algorithms).")

    include_directories(${MPI_C_INCLUDE_PATH}) # MPI C-bindings

    # Compile the optimisation library with MPI support (parallel algorithms)
    add_library(hop SHARED
      ${HOP_LIBRARY_SOURCES}
      src/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm.cpp
      src/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm/parallelStandardParticleSwarmOptimisation2011.cpp
    )

    # Link against third party libraries
    target_link_libraries(hop ${MPI_C_LIBRARIES}) # MPI C-bindings
  else()
    message(STATUS "NOTICE: Compile without MPI support (serial algorithms only).")

    # Compile the optimisation library without MPI support (serial algorithms only)
    add_library(hop SHARED
      ${HOP_LIBRARY_SOURCES}
    )
  endif()

  # Link against third party libraries
  target_link_libraries(hop ${ARMADILLO_LIBRARIES}) # Armadillo C++

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
      include_directories(${PROJECT_SOURCE_DIR}/test) # HOP tests

      # Compile the optimisation library tests
      # All paths must start with "test/"
      add_executable(libTest
        test/main.cpp
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
        test/helper/testGeometry.cpp
        test/helper/testRandom.cpp
        test/helper/testString.cpp
      )

      # Add HOP library directory
      link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) # HOP

      # Link against HOP library
      target_link_libraries(libTest hop) # HOP

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

  set_property(TARGET hop PROPERTY VERSION ${HOP_VERSION})
  set_property(TARGET hop PROPERTY SOVERSION ${HOP_MAJOR})

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)

  install(TARGETS hop
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR})

#  include(CMakePackageConfigHelpers)
#  write_basic_package_version_file("${PROJECT_SOURCE_DIR}/HOPConfigVersion.cmake" VERSION ${HOP_VERSION} COMPATIBILITY AnyNewerVersion)
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
  include/hop
  include/hop_bits/distanceFunction.hpp
  include/hop_bits/distanceFunction/euclideanDistance.hpp
  include/hop_bits/optimisationProblem.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/attractiveSectorFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/bentCigarFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/buecheRastriginFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/compositeGriewankRosenbrockFunctionF8F2.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/differentPowersFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/discusFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/ellipsoidalFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/ellipsoidalFunctionRotated.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/gallaghersGaussian101mePeaksFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/gallaghersGaussian21hiPeaksFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/katsuuraFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/linearSlope.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/lunacekBiRastriginFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rastriginFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rastriginFunctionRotated.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rosenbrockFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/rosenbrockFunctionRotated.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/schaffersF7Function.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/schaffersF7FunctionIllConditioned.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/schwefelFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/sharpRidgeFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/sphereFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/stepEllipsoidalFunction.hpp
  include/hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/weierstrassFunction.hpp
  include/hop_bits/optimisationProblem/kinematicMachine.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prpr.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prrr.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3prus.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_3pups.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_4prus.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_6prus.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_6pups.hpp
  include/hop_bits/optimisationProblem/kinematicMachine/multiLevelStewartPlatform.hpp
  include/hop_bits/optimisationProblem/nkModel.hpp
  include/hop_bits/optimisationAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp
  include/hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/multiResolutionGridSearch.hpp
  include/hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/randomSearch.hpp
  include/hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.hpp
  include/hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hookeJeevesAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/simulatedAnnealing.hpp
  include/hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/populationBasedAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp
  include/hop_bits/optimisationAlgorithm/populationBasedAlgorithm/roleBasedImitationAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/populationBasedAlgorithm/standardParticleSwarmOptimisation2011.hpp
  include/hop_bits/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm.hpp
  include/hop_bits/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm/parallelStandardParticleSwarmOptimisation2011.hpp
  include/hop_bits/propertiesAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis/fitnessDistanceCorrelationAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis/directLipschitzContinuityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis/linearOrdinaryLeastSquares.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp
  include/hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis/quadraticOrdinaryLeastSquares.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis/directAdditiveSeparabilityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp
  include/hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis/directLinearProportionalityAnalysis.hpp
  include/hop_bits/propertiesEstimation.hpp
  include/hop_bits/propertiesEstimation/maximumLikelihoodEstimation.hpp
  include/hop_bits/helper/cereal.hpp
  include/hop_bits/helper/combinatorics.hpp
  include/hop_bits/helper/geometry.hpp
  include/hop_bits/helper/printable.hpp
  include/hop_bits/helper/quasiRandomSequence.hpp
  include/hop_bits/helper/random.hpp
  include/hop_bits/helper/rng.hpp
  include/hop_bits/helper/string.hpp
  include/hop_bits/helper/unordered_map.hpp
)
