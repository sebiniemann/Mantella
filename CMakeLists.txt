cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Retrieves the version numbers from "include/mantella", avoiding possible inconsistency.
file(STRINGS "./include/mantella" MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MAJOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH "${MANTELLA_HEADER_CONTENTS}")
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

project(Mantella VERSION "${MANTELLA_VERSION}" LANGUAGES CXX C)

# Adds additional cmake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(SUPPORT_SIMD "Add SSE2, SSE3, SSE4, AVX, etc. support" ON)
option(SUPPORT_OPENMP "Add OpenMP support" ON)
option(SUPPORT_MPI "Add MPI support" OFF)
# Development build options
option(BUILD_TESTS "Build tests" OFF)

mark_as_advanced(
  BUILD_TESTS
)

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endif()
  
message(STATUS "") 
message(STATUS "Building Mantella (version: ${MANTELLA_VERSION}).")

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "- Setting build to 'Release'.")
  message(STATUS "  - Use 'cmake ... -DCMAKE_BUILD_TYPE=[Release|Debug]' to specify this.")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# --------------------
# FINDING DEPENDENCIES
# --------------------

message(STATUS "") 
message(STATUS "Finding dependencies.")

set(REQUIRED_ARMADILLO_VERSION "7.300.0")
find_package(Armadillo QUIET)
    file(STRINGS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/config.hpp" ARMA_USE_WRAPPER REGEX "^[ ]*#define ARMA_USE_WRAPPER")
if (NOT ARMADILLO_FOUND)
  # Unlike `find_package(Armadillo ...)` suggests, Armadillo can be used if its runtime library is missing, as long as `ARMA_USE_WRAPPER` is undefined/outcommented inside Armadillo's configuration file. In this case, we can also directly link against BLAS/LAPACK libraries instead.
  # However, we need to make sure that a missing runtime library was the only reason for `ARMADILLO_FOUND` to be unset.
  # Currently, we only support OpenBLAS as a replacement for Armadillo's runtime library.
  if (NOT ARMADILLO_INCLUDE_DIR)
    message(FATAL_ERROR "Could NOT find Armadillo includes.")
  elseif (ARMADILLO_VERSION_STRING VERSION_LESS REQUIRED_ARMADILLO_VERSION)
    message(FATAL_ERROR "Found unsuitable Armadillo version ${ARMADILLO_VERSION_STRING}, but required is at least ${REQUIRED_ARMADILLO_VERSION} (found at ${ARMADILLO_INCLUDE_DIR}).")
  else()
    file(STRINGS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/config.hpp" ARMA_USE_WRAPPER REGEX "^[ ]*#define ARMA_USE_WRAPPER")
    if (ARMA_USE_WRAPPER AND NOT ARMADILLO_LIBRARY)
      message(FATAL_ERROR "Armadillo's runtime library could not be found, undefine/outcomment `ARMA_USE_WRAPPER` inside Armadillo's configuration file, if you want to link directly againt OpenBLAS instead.")
    endif()
    
    message(STATUS "Found Armadillo: ${ARMADILLO_INCLUDE_DIR} (found suitable version ${ARMADILLO_VERSION_STRING}, minimum required is ${REQUIRED_ARMADILLO_VERSION}).")
    find_package(OpenBLAS REQUIRED)
  endif()
else()
  message(STATUS "Found Armadillo: ${ARMADILLO_LIBRARIES} (found suitable version ${ARMADILLO_VERSION_STRING}, minimum required is ${REQUIRED_ARMADILLO_VERSION}).")
endif()

if (SUPPORT_MPI)
  message(STATUS "- Adding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=Off' to exclude this.")
  find_package(MPI REQUIRED)
else()
  message(STATUS "- Excluding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=ON' to add this.")
endif()

if (SUPPORT_OPENMP)
  message(STATUS "- Trying to add OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OPENMP=Off' to exclude this.")

  # Makes sure that `MAXIMAL_NUMBER_OF_THREADS` contains a sane value.
  if (NOT MAXIMAL_NUMBER_OF_THREADS OR NOT MAXIMAL_NUMBER_OF_THREADS MATCHES "^[1-9][0-9]*$")
    message(STATUS "  - Trying to determine the number of cores.")
  
    include(ProcessorCount)
    ProcessorCount(MAXIMAL_NUMBER_OF_THREADS)
    message(STATUS "    - Found ${MAXIMAL_NUMBER_OF_THREADS} core(s).")
  endif()

  if (MAXIMAL_NUMBER_OF_THREADS EQUAL 0)
    message(STATUS "- Excluding OpenMP support as the maximal number of threads to support was either not automatically determined or manually set to an integer less than 1.")
    message(STATUS "  - Use 'cmake ... -DMAXIMAL_NUMBER_OF_THREADS=...' to specify the maximal number of threads (>= 1) to support manually.")
    set(SUPPORT_OPENMP OFF)
  else()
    find_package(OpenMP)
    if (OPENMP_FOUND)
      message(STATUS "  - Adding OpenMP support (optimising for ${MAXIMAL_NUMBER_OF_THREADS} threads(s)).")
    message(STATUS "  - Use 'cmake ... -DMAXIMAL_NUMBER_OF_THREADS=...' to specify the maximal number of threads (>= 1) manually.")
      message(STATUS "  - Alternatively, set `#define MAXIMAL_NUMBER_OF_THREADS ...` in your source code, prior to including Mantella the first time.")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    else()
      message(STATUS "  - OpenMP is not supported by your compiler.")
      set(SUPPORT_OPENMP OFF)
    endif()
  endif()
else()
  message(STATUS "- Excluding OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OPENMP=ON' to add this.")
endif()

# Makes sure that `MAXIMAL_NUMBER_OF_THREADS` contains a sane value, even if OpenMP is not to be supported.
if (NOT SUPPORT_OPENMP)
  set(MAXIMAL_NUMBER_OF_THREADS 1)
endif()

# -------------------------
# CONFIGURING LIBRARY BUILD
# -------------------------

message(STATUS "") 
message(STATUS "Configuring library build.")

configure_file(${PROJECT_SOURCE_DIR}/include/mantella_bits/config.hpp.cmake ${PROJECT_SOURCE_DIR}/include/mantella_bits/config.hpp)

# All paths must start with "src/"
add_library(mantella SHARED
  # Configuration
  src/config.cpp
  
  # Assertion
  src/assert.cpp

  # Random number generator
  src/randomNumberGenerator.cpp
  
  # MPI
  src/mpi.cpp
  
  # Armadillo
  src/armadillo.cpp
  
  # Mathematical functions
  src/algebra.cpp
  src/combinatorics.cpp
  src/geometry.cpp
  src/numberTheory.cpp
  src/numericalAnalysis.cpp
  src/probability.cpp
  
  # Orbital Mechanics
  src/orbitalMechanics.cpp
  
  # Machine learning functions
  src/samplesAnalysis.cpp
  src/samplesSelection.cpp
  
  # Optimisation problems
  src/optimisationProblem.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/bentCigarFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/buecheRastriginFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/compositeGriewankRosenbrockFunctionF8F2.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/differentPowersFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/discusFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunctionRotated.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian21hiPeaksFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/katsuuraFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/linearSlope.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunctionRotated.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunctionRotated.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/schaffersF7Function.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/schaffersF7FunctionIllConditioned.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/schwefelFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/sharpRidgeFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/sphereFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/stepEllipsoidalFunction.cpp
  src/optimisationProblem/blackBoxOptimisationBenchmark/weierstrassFunction.cpp
  src/optimisationProblem/kinematicallyRedundantMachines.cpp
  src/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine3PRPR.cpp
  src/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine3PRRR.cpp
  src/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine6PRUS.cpp
  src/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine6PUPS.cpp

  # Optimisation algorithms
  src/optimisationAlgorithm.cpp
  src/optimisationAlgorithm/hillClimbing.cpp
  src/optimisationAlgorithm/hookeJeevesAlgorithm.cpp
  src/optimisationAlgorithm/randomSearch.cpp
  src/optimisationAlgorithm/simulatedAnnealing.cpp
  src/optimisationAlgorithm/populationBasedOptimisationAlgorithm.cpp
  src/optimisationAlgorithm/populationBasedOptimisationAlgorithm/differentialEvolution.cpp
  src/optimisationAlgorithm/populationBasedOptimisationAlgorithm/particleSwarmOptimisation.cpp
)
set_property(TARGET mantella PROPERTY CXX_STANDARD 14)
set_property(TARGET mantella PROPERTY CXX_STANDARD_REQUIRED ON)

if (SUPPORT_SIMD)
  message(STATUS "- Adding SSE3, SSE4, AVX, ... support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_SIMD=Off' to exclude this.")
  target_compile_options(mantella PRIVATE -march=native)
else()
  message(STATUS "- Excluding SSE3, SSE4, AVX, ... support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_SIMD=ON' to add this.")
endif()

message(STATUS "- Using level 3 code optimisation.")
target_compile_options(mantella PRIVATE -O3)

target_include_directories(mantella PUBLIC 
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(mantella SYSTEM PUBLIC ${ARMADILLO_INCLUDE_DIR})
if (ARMADILLO_LIBRARIES)
  target_link_libraries(mantella PUBLIC ${ARMADILLO_LIBRARIES})
else()
  target_link_libraries(mantella PUBLIC ${OPENBLAS_LIBRARIES})
endif()

if (SUPPORT_MPI)
  target_include_directories(mantella SYSTEM PUBLIC ${MPI_INCLUDE_PATH})
  target_link_libraries(mantella PUBLIC ${MPI_C_LIBRARIES})
endif()

# ----------------------
# CONFIGURING UNIT TESTS
# ----------------------

if (BUILD_TESTS)
  message(STATUS "") 
  message(STATUS "Configuring unit tests.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=Off' to exclude this.")
    
  # Additional requirement
  find_package(Catch REQUIRED) # Catch (header only)
  
  # All paths must start with "test/"
  add_executable(mantellaTests
    test/main.cpp
    
    # Test framework extensions
    test/catchHelper.cpp
    test/testCatchHelper.cpp
    
    # Configurations
    test/testConfig.cpp
    
    # Assertions
    test/testAssert.cpp

    # Random number generator
    test/testRandomNumberGenerator.cpp
    
    # MPI
    test/testMpi.cpp
    
    # Armadillo
    test/testArmadillo.cpp
  
    # Mathematical functions
    test/testAlgebra.cpp
    test/testCombinatorics.cpp
    test/testGeometry.cpp
    test/testNumberTheory.cpp
    test/testNumericalAnalysis.cpp
    test/testProbability.cpp
    
    # Orbital Mechanics
    test/testOrbitalMechanics.cpp
  
    # Orbital Mechanics
    test/testOrbitalMechanics.cpp
  
    # Orbital Mechanics
    test/testOrbitalMechanics.cpp
  
    # Machine learning functions
    test/testSamplesAnalysis.cpp
    test/testSamplesSelection.cpp

    # Optimisation problems
    test/testOptimisationProblem.cpp
    test/optimisationProblem/testBlackBoxOptimisationBenchmark.cpp
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
    test/optimisationProblem/testKinematicallyRedundantMachines.cpp
    test/optimisationProblem/kinematicallyRedundantMachines/testParallelKinematicMachine3PRPR.cpp
    test/optimisationProblem/kinematicallyRedundantMachines/testParallelKinematicMachine3PRRR.cpp
    test/optimisationProblem/kinematicallyRedundantMachines/testParallelKinematicMachine6PRUS.cpp
    test/optimisationProblem/kinematicallyRedundantMachines/testParallelKinematicMachine6PUPS.cpp
    
    # Optimisation algorithms
    test/testOptimisationAlgorithm.cpp
    test/optimisationAlgorithm/testHillClimbing.cpp
    test/optimisationAlgorithm/testHookeJeevesAlgorithm.cpp
    test/optimisationAlgorithm/testRandomSearch.cpp
    test/optimisationAlgorithm/testSimulatedAnnealing.cpp
    test/optimisationAlgorithm/testPopulationBasedOptimisationAlgorithm.cpp
    test/optimisationAlgorithm/populationBasedOptimisationAlgorithm/testDifferentialEvolution.cpp
    test/optimisationAlgorithm/populationBasedOptimisationAlgorithm/testParticleSwarmOptimisation.cpp
  )
  set_property(TARGET mantellaTests PROPERTY CXX_STANDARD 14)
  set_property(TARGET mantellaTests PROPERTY CXX_STANDARD_REQUIRED ON)
  
  message(STATUS "- Adding debugging symbols.")
  target_compile_options(mantellaTests PRIVATE -g)
  
  if (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "- Adding code coverage measurement.")
    target_compile_options(mantella PRIVATE --coverage)
    target_link_libraries(mantella PRIVATE --coverage)
  endif()
  
  message(STATUS "- Displaying compiler warnings (globally).")
  if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(
      -Wall
      -Wextra
      -Wno-unknown-pragmas)
  else()
    add_compile_options(
      -Weverything
      -fsanitize=undefined
      -Wno-c++98-compat
      -Wno-weak-vtables
      -Wno-exit-time-destructors
      -Wno-global-constructors
      -Wno-unknown-warning-option)
  endif()
  
  message(STATUS "- Handling warnings as errors (globally).")
  add_compile_options(
    -Wno-padded
    -Wno-unused-parameter
    -Wno-c++98-compat-pedantic
    -Werror)
  
  target_include_directories(mantellaTests PRIVATE ${CATCH_INCLUDE_DIRS})
  target_link_libraries(mantellaTests PUBLIC mantella)
else()
  message(STATUS "") 
  message(STATUS "Skipping unit tests.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=ON' to add this.")
endif()

# -------------
# BUILD LIBRARY
# -------------

message(STATUS "")
message(STATUS "Configuring library installation.")

if (NOT INSTALL_INCLUDE_DIR)
  set(INSTALL_INCLUDE_DIR "include")
endif()

set_property(TARGET mantella PROPERTY VERSION ${MANTELLA_VERSION})
set_property(TARGET mantella PROPERTY SOVERSION ${MANTELLA_VERSION_MAJOR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)

if (NOT INSTALL_LIB_DIR)
  set(INSTALL_LIB_DIR "lib")
endif()

if (NOT INSTALL_BIN_DIR)
  set(INSTALL_BIN_DIR "bin")
endif()

install(TARGETS mantella
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR})
  
message(STATUS "")
message(STATUS "Noticable variables:")
message(STATUS "- CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "- SUPPORT_SIMD = ${SUPPORT_SIMD}")
message(STATUS "- SUPPORT_OPENMP = ${SUPPORT_OPENMP}")
message(STATUS "- SUPPORT_MPI = ${SUPPORT_MPI}")
message(STATUS "- ARMADILLO_LIBRARIES = ${ARMADILLO_LIBRARIES}")
message(STATUS "- INSTALL_INCLUDE_DIR = ${INSTALL_INCLUDE_DIR}")
message(STATUS "- INSTALL_LIB_DIR = ${INSTALL_LIB_DIR}")
message(STATUS "- INSTALL_BIN_DIR = ${INSTALL_BIN_DIR}")
message(STATUS "")
