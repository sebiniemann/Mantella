cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Mantella, C++ optimisation framework
project(Mantella CXX C)

# Retrieves the version numbers from "include/mantella", avoiding possible inconsistency.
file(STRINGS "${PROJECT_SOURCE_DIR}/include/mantella" MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MAJOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH "${MANTELLA_HEADER_CONTENTS}")
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

# Adds additional cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(SUPPORT_SIMD "Add SSE2, SSE3, SSE4, AVX, etc. support" ON)
option(SUPPORT_OMP "Add OpenMP support" ON)
option(SUPPORT_MPI "Add MPI support" OFF)
# Development build options
option(USE_CCACHE "Use ccache" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(MEASURE_CODE_COVERAGE "Measure code coverage" OFF)

mark_as_advanced(
  BUILD_TESTS
  MEASURE_CODE_COVERAGE
)

message(STATUS "") 
message(STATUS "Building Mantella (version: ${MANTELLA_VERSION}).")

# ------------------------
# CHECKING PREREQUIREMENTS
# ------------------------

message(STATUS "") 
message(STATUS "Checking prerequirements.")

set(REQUIRED_ARMADILLO_VERSION "6.100.0")
find_package(Armadillo QUIET)
    file(STRINGS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/config.hpp" ARMA_USE_WRAPPER REGEX "^[ ]*#define ARMA_USE_WRAPPER")
if(NOT ARMADILLO_FOUND)
  # Unlike `find_package(Armadillo ...)` suggests, Armadillo can be used if its runtime library is missing, as long as `ARMA_USE_WRAPPER` is undefined/outcommented inside Armadillo's configuration file. In this case, we can also directly link against BLAS/LAPACK libraries instead.
  # However, we need to make sure that a missing runtime library was the only reason for `ARMADILLO_FOUND` to be unset.
  # Currently, we only support OpenBLAS as a replacement for Armadillo's runtime library.
  if(NOT ARMADILLO_INCLUDE_DIR)
    message(FATAL_ERROR "Could NOT find Armadillo includes.")
  elseif(ARMADILLO_VERSION_STRING VERSION_LESS REQUIRED_ARMADILLO_VERSION)
    message(FATAL_ERROR "Found unsuitable Armadillo version ${ARMADILLO_VERSION_STRING}, but required is at least ${REQUIRED_ARMADILLO_VERSION} (found at ${ARMADILLO_INCLUDE_DIR}).")
  else()
    file(STRINGS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/config.hpp" ARMA_USE_WRAPPER REGEX "^[ ]*#define ARMA_USE_WRAPPER")
    if(ARMA_USE_WRAPPER AND NOT ARMADILLO_LIBRARY)
      message(FATAL_ERROR "Armadillo's runtime library could not be found, undefine/outcomment `ARMA_USE_WRAPPER` inside Armadillo's configuration file, if we should link directly againt OpenBLAS instead.")
    endif()
    
    message(STATUS "Found Armadillo: ${ARMADILLO_INCLUDE_DIR} (found suitable version ${ARMADILLO_VERSION_STRING}, minimum required is ${REQUIRED_ARMADILLO_VERSION}).")
    find_package(OpenBLAS REQUIRED)
  endif()
else()
  message(STATUS "Found Armadillo: ${ARMADILLO_LIBRARIES} (found suitable version ${ARMADILLO_VERSION_STRING}, minimum required is ${REQUIRED_ARMADILLO_VERSION}).")
endif()
include_directories(${ARMADILLO_INCLUDE_DIR})
  
if(SUPPORT_MPI)
  find_package(MPI REQUIRED)
  include_directories(${MPI_INCLUDE_PATH})
endif()

# --------------------
# CONFIGURING COMPILER
# --------------------

message(STATUS "") 
message(STATUS "Configuring compiler.")

# Adds Clang detection
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(CMAKE_COMPILER_IS_CLANGXX)
  message(STATUS "- Detected Clang compiler.")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "- Detected GCC compiler.")
else()
  message(STATUS "- Detected neither Clang, nor GCC, but '${CMAKE_CXX_COMPILER_ID}' instead. Compiler flags may not be optimal.")
endif()

if(USE_CCACHE)
  find_program(CCACHE_FOUND ccache)
  message(STATUS "- Trying to use ccache to speed up recompilations.")
  message(STATUS "  - Use 'cmake ... -DUSE_CCACHE=Off' to exclude this.")
  if(CCACHE_FOUND)
    message(STATUS "  - Using ccache.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  else()
    message(STATUS "  - ccache was no found on your system.")
  endif()
else()
  message(STATUS "- Excluding ccache usage.")
  message(STATUS "  - Use 'cmake ... -DUSE_CCACHE=On' to add this.")
endif()

# ----------------------
# SETTING COMPILER FLAGS
# ----------------------

message(STATUS "") 
message(STATUS "Setting compiler flags.")

message(STATUS "- Enabling C++11 features.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(SUPPORT_SIMD)
  message(STATUS "- Adding SSE3, SSE4, AVX, ... support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_SIMD=Off' to exclude this.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
else()
  message(STATUS "- Excluding SSE3, SSE4, AVX, ... support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_SIMD=On' to add this.")
endif()

message(STATUS "- Using level 3 code optimisation.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "- Setting build to to 'Release'.")
  message(STATUS "  - Use 'cmake ... -DCMAKE_BUILD_TYPE=[Release|Debug]' to specify this.")
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(SUPPORT_OMP)
  message(STATUS "- Trying to add OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OMP=Off' to exclude this.")
  find_package(OpenMP)
  if (OPENMP_FOUND)
    message(STATUS "  - Added OpenMP support.")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  else()
    message(STATUS "  - OpenMP is not supported by your compiler.")
  endif()
else()
  message(STATUS "- Excluding OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OMP=On' to add this.")
endif()

# ----------------
# BUILDING LIBRARY
# ----------------

message(STATUS "") 
message(STATUS "Configuring library build.")

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()

if(NOT ARCHIVE_OUTPUT_DIRECTORY)
  set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()
   
# Include Mantella
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)

if(SUPPORT_MPI)
  message(STATUS "- Adding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=Off' to exclude this.")
else()
  message(STATUS "- Excluding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=On' to add this.")
endif()
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
  src/statistics.cpp
  src/topology.cpp
  
  # Machine learning
  src/samplesSelection.cpp
  src/samplesAnalysis.cpp
  
  # Optimisation problems
  src/optimisationProblem.cpp
  src/optimisationProblem/benchmarkOptimisationProblem.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/bentCigarFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/buecheRastriginFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/compositeGriewankRosenbrockFunctionF8F2.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/differentPowersFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/discusFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunctionRotated.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian21hiPeaksFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/katsuuraFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/linearSlope.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/rastriginFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/rastriginFunctionRotated.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunctionRotated.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/schaffersF7Function.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/schaffersF7FunctionIllConditioned.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/schwefelFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/sharpRidgeFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/sphereFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/stepEllipsoidalFunction.cpp
  src/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/weierstrassFunction.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine3PRPR.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine3PRRR.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine6PRUS.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine6PUPS.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/multiLevelStewartPlatform.cpp
  src/optimisationProblem/kinematicallyRedundantMachineProblem/poseInaccuracy.cpp
  src/optimisationProblem/surrogateModel.cpp
  src/optimisationProblem/surrogateModel/polynomialFunctionModel.cpp
  # src/optimisationProblem/surrogateModel/radialBasisFunctionModel.cpp
  # src/optimisationProblem/surrogateModel/krigingModel.cpp
  # src/optimisationProblem/surrogateModel/multivariateAdaptiveRegressionSplinesModel.cpp
  # src/optimisationProblem/surrogateModel/artificialNeuralNetwork.cpp

  # Optimisation algorithms
  src/optimisationAlgorithm.cpp
  src/optimisationAlgorithm/gridSearch.cpp
  src/optimisationAlgorithm/randomSearch.cpp
  src/optimisationAlgorithm/hillClimbing.cpp
  src/optimisationAlgorithm/hookeJeevesAlgorithm.cpp
  src/optimisationAlgorithm/simulatedAnnealing.cpp
  src/optimisationAlgorithm/particleSwarmOptimisation.cpp
  
  # Cluster analyses
  src/clusterAnalysis.cpp
  src/clusterAnalysis/basicSequentialAlgorithmicScheme.cpp
  src/clusterAnalysis/twoThresholdSequentialAlgorithmicScheme.cpp
)

# Linking against prerequirements
if(ARMADILLO_LIBRARIES)
  target_link_libraries(mantella ${ARMADILLO_LIBRARIES})
else()
  target_link_libraries(mantella ${OPENBLAS_LIBRARIES})
endif()

if(SUPPORT_MPI)
  target_link_libraries(mantella ${MPI_C_LIBRARIES})
endif()

# -------------------
# BUILDING UNIT TESTS
# -------------------

if(BUILD_TESTS)
  message(STATUS "") 
  message(STATUS "Configuring unit tests.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=Off' to exclude this.")

  if(MEASURE_CODE_COVERAGE)
    message(STATUS "- Adding code coverage measurement.")
    message(STATUS "  - Use 'cmake ... -DMEASURE_CODE_COVERAGE=Off' to exclude this.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  else()
    message(STATUS "- Excluding code coverage measurement.")
    message(STATUS "  - Use 'cmake ... -DMEASURE_CODE_COVERAGE=On' to add this.")
  endif()
  
  message(STATUS "- Forcing debug build and deactivating code optimisation.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  set(CMAKE_BUILD_TYPE "Debug")
  
  if(CMAKE_COMPILER_IS_CLANGXX OR CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "- Displaying compiler warnings.")
    if(CMAKE_COMPILER_IS_CLANGXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
      # Ignore warnings for unknown options, as we make use of options introduced by more recent Clang versions.
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option")
      
      # Ignore warnings resulting from <mpi.h>
      if(SUPPORT_MPI)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-extra-semi")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cast-align")
      endif()
    elseif(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
    
    message(STATUS "- Handling warnings as errors.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()
  
  # if(CMAKE_COMPILER_IS_CLANGXX)
    # message(STATUS "- Including a undefined behavior checker and memory error detector.")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize=address")
  # endif()
    
  if(NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
  endif()

  # Check additional prerequirements
  find_package(Catch REQUIRED) # Catch (header only)
  include_directories(${CATCH_INCLUDE_DIRS}) # Catch
  
  # Include catch extensions
  include_directories(${PROJECT_SOURCE_DIR}/test)
  
  # Compile tests
  # All paths must start with "test/"
  add_executable(mantellaTest
    test/main.cpp
    
    # Test framework extension
    test/catchExtension.cpp
    test/testCatchExtension.cpp
    
    # Configuration
    test/testConfig.cpp
    
    # Assertion
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
    test/testStatistics.cpp
    test/testTopology.cpp
  
    # # Machine learning
    # test/testSamplesSelection.cpp
    # test/testSamplesAnalysis.cpp

    # Optimisation problems
    test/testOptimisationProblem.cpp
    test/optimisationProblem/testBenchmarkOptimisationProblem.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/testBlackBoxOptimisationBenchmark.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testAttractiveSectorFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testBentCigarFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testBuecheRastriginFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testDifferentPowersFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testDiscusFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testEllipsoidalFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testEllipsoidalFunctionRotated.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testGallaghersGaussian101mePeaksFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testGallaghersGaussian21hiPeaksFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testKatsuuraFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testLinearSlope.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testLunacekBiRastriginFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testRastriginFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testRastriginFunctionRotated.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testRosenbrockFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testRosenbrockFunctionRotated.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testSchaffersF7Function.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testSchaffersF7FunctionIllConditioned.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testSchwefelFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testSharpRidgeFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testSphereFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testStepEllipsoidalFunction.cpp
    test/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/testWeierstrassFunction.cpp
    test/optimisationProblem/testKinematicallyRedundantMachineProblem.cpp
    test/optimisationProblem/kinematicallyRedundantMachineProblem/testMachineModel.cpp
    test/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/testParallelKinematicMachine3PRPR.cpp
    test/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/testParallelKinematicMachine3PRRR.cpp
    test/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/testParallelKinematicMachine6PRUS.cpp
    test/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/testParallelKinematicMachine6PUPS.cpp
    # test/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/testMultiLevelStewartPlatform.cpp
    # test/optimisationProblem/kinematicallyRedundantMachineProblem/testPoseInaccuracy.cpp
    # test/optimisationProblem/testSurrogateModel.cpp
    # test/optimisationProblem/surrogateModel/testPolynomialFunctionModel.cpp
    # test/optimisationProblem/surrogateModel/testRadialBasisFunctionModel.cpp
    # test/optimisationProblem/surrogateModel/testKrigingModel.cpp
    # test/optimisationProblem/surrogateModel/testMultivariateAdaptiveRegressionSplinesModel.cpp
    # test/optimisationProblem/surrogateModel/testArtificialNeuralNetwork.cpp
    
    # # Optimisation algorithms
    # test/testOptimisationAlgorithm.cpp
    # test/optimisationAlgorithm/testRandomSearch.cpp
    # test/optimisationAlgorithm/testGridSearch.cpp
    # test/optimisationAlgorithm/testHillClimbing.cpp
    # test/optimisationAlgorithm/testHookeJeevesAlgorithm.cpp
    # test/optimisationAlgorithm/testSimulatedAnnealing.cpp
    # test/optimisationAlgorithm/testParticleSwarmOptimisation.cpp
  
    # # Cluster analyses
    # test/testClusterAnalysis.cpp
    # test/clusterAnalysis/testBasicSequentialAlgorithmicScheme.cpp
    # test/clusterAnalysis/testTwoThresholdSequentialAlgorithmicScheme.cpp
  )

  # Link against prerequirements
  target_link_libraries(mantellaTest mantella)
  
  if(ARMADILLO_LIBRARIES)
    target_link_libraries(mantellaTest ${ARMADILLO_LIBRARIES})
  else()
    target_link_libraries(mantellaTest ${OPENBLAS_LIBRARIES})
  endif()

  if(SUPPORT_MPI)
    target_link_libraries(mantellaTest ${MPI_C_LIBRARIES})
  endif()
else()
  message(STATUS "") 
  message(STATUS "Skipping unit tests.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=On' to add this.")
endif()

# -------------
# BUILD LIBRARY
# -------------

message(STATUS "")
message(STATUS "Configuring library installation.")

if(NOT INSTALL_INCLUDE_DIR)
  set(INSTALL_INCLUDE_DIR "include")
endif()

set_property(TARGET mantella PROPERTY VERSION ${MANTELLA_VERSION})
set_property(TARGET mantella PROPERTY SOVERSION ${MANTELLA_VERSION_MAJOR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)

if(NOT INSTALL_LIB_DIR)
  set(INSTALL_LIB_DIR "lib")
endif()

install(TARGETS mantella
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR})

message(STATUS "")
message(STATUS "Noticable CMAKE variables:")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}.")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "- ARCHIVE_OUTPUT_DIRECTORY = ${ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "- EXECUTABLE_OUTPUT_PATH = ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "- CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "- INSTALL_INCLUDE_DIR = ${INSTALL_INCLUDE_DIR}")
message(STATUS "- INSTALL_LIB_DIR = ${INSTALL_LIB_DIR}")
message(STATUS "")
