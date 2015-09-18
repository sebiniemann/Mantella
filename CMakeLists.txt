cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Mantella, C++ optimisation framework
project(Mantella CXX C)

# Retrieves the version numbers from "include/mantella"
file(STRINGS "${PROJECT_SOURCE_DIR}/include/mantella" MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MAJOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH "${MANTELLA_HEADER_CONTENTS}")
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

# Adds additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Adds Clang detection
if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

option(SUPPORT_SIMD "Add SSE2, SSE3, SSE4, AVX, etc. support" ON)
option(SUPPORT_OMP "Add OpenMP support" ON)
option(SUPPORT_MPI "Add MPI support" OFF)
# Development build options
option(BUILD_TESTS "Build tests" OFF)
option(MEASURE_CODE_COVERAGE "Measure code coverage" OFF)
option(RUN_IWYU "Run include-what-you-use" OFF)

mark_as_advanced(
 BUILD_TESTS
 MEASURE_CODE_COVERAGE
 RUN_IWYU
)

message(STATUS "") 
message(STATUS "Building Mantella (version: ${MANTELLA_VERSION}).")

# ------------------------
# CHECKING PREREQUIREMENTS
# ------------------------

message(STATUS "") 
message(STATUS "Checking prerequirements.")

set(REQUIRED_ARMADILLO_VERSION "5.100.2")
# Whenever or not Armadillo should be used together with its runtime library, or should be linked directly with LAPACK/BLAS can by deduced by ARMA_USE_WRAPPER.
# **Note:** The Armadillo header files are always required.
find_package(Armadillo QUIET)
file(STRINGS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/config.hpp" ARMA_USE_WRAPPER REGEX "^[ ]*#define ARMA_USE_WRAPPER")
if(ARMA_USE_WRAPPER)
  # Using the Armadillo runtime library (this is the recommended way).
  find_package(Armadillo ${REQUIRED_ARMADILLO_VERSION} REQUIRED)
else()
  # Using Armadillo's headers, but directly linking to OpenBLAS.
  # Since this would also work if libarmadillo.lib was never build, but it is always required by find_package(...), we have to check all pieces manually. *sigh ...*
  if(NOT ARMADILLO_INCLUDE_DIR)
    message(FATAL_ERROR "Could NOT find Armadillo includes.")
  elseif(ARMADILLO_VERSION_STRING VERSION_LESS REQUIRED_ARMADILLO_VERSION)
    message(FATAL_ERROR "Found unsuitable Armadillo version ${ARMADILLO_VERSION_STRING}, but required is at least ${REQUIRED_ARMADILLO_VERSION} (found at ${ARMADILLO_INCLUDE_DIR}).")
  else()
    message(STATUS "Found Armadillo: ${ARMADILLO_INCLUDE_DIR} (found suitable version ${ARMADILLO_VERSION_STRING}, minimum required is ${REQUIRED_ARMADILLO_VERSION}).")
    find_package(OpenBLAS REQUIRED)
  endif()
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

if(RUN_IWYU)
  message(STATUS "- Running IWYU (instead of generating an excecutable/library).")
  message(STATUS "  - Use 'cmake ... -DRUN_IWYU=Off' to exclude this.")
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(include-what-you-use CLANG)
  CMAKE_FORCE_CXX_COMPILER(include-what-you-use CLANG)
else()
  # Use ccache to speed up recompilation, if possible. 
  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    message(STATUS "- Using ccache to speed up recompilations.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    
    if(CMAKE_COMPILER_IS_CLANGXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
    endif()
  endif()
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

if(MEASURE_CODE_COVERAGE)
  message(STATUS "- Adding code coverage measurements, forcing debug build and deactivating code optimisation.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
  set(CMAKE_BUILD_TYPE "Debug")
else()
  message(STATUS "- Using level 3 code optimisation.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

if(SUPPORT_OMP)
  message(STATUS "- Adding OpenMP support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_OMP=Off' to exclude this.")
  find_package(OpenMP)
  if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  else()
    message(STATUS "- OpenMP is not supported by your compiler.")
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
  
  # Asserts
  src/helper/assert.cpp

  # Random number generator
  src/helper/rng.cpp
  
  # MPI
  src/helper/mpi.cpp
  
  # Helper
  src/helper/geometry.cpp
  src/helper/printable.cpp
  src/helper/quasiRandomSequence.cpp
  src/helper/random.cpp
  src/helper/regression.cpp
  src/helper/setTheory.cpp
  src/helper/statistic.cpp
  src/helper/unorderedContainer.cpp

  # Distance functions
  src/distanceFunction.cpp
  src/distanceFunction/pNorm.cpp
  src/distanceFunction/mahalabonisDistance.cpp
  
  # Regression functions
  src/regressionFunction.cpp
  src/regressionFunction/constantPolynomialRegressionFunction.cpp
  src/regressionFunction/linearPolynomialRegressionFunction.cpp
  src/regressionFunction/quadraticPolynomialRegressionFunction.cpp
  src/regressionFunction/cubicPolynomialRegressionFunction.cpp

  # Correlation functions
  src/correlationFunction.cpp
  src/correlationFunction/polynomialCorrelationFunction.cpp
  src/correlationFunction/splineCorrelationFunction.cpp
  src/correlationFunction/exponentialCorrelationFunction.cpp
  src/correlationFunction/gaussianCorrelationFunction.cpp
  
  # Radial basis functions
  src/radialBasisFunction.cpp
  src/radialBasisFunction/polyharmonicSplineRadialBasisFunction.cpp
  src/radialBasisFunction/multiquadraticRadialBasisFunction.cpp
  src/radialBasisFunction/gaussianRadialBasisFunction.cpp
  
  # Samples selections
  src/samplesSelection.cpp
  src/samplesSelection/bestFittingSamplesSelection.cpp
  src/samplesSelection/bestNeighbourhoodSamplesSelection.cpp
  src/samplesSelection/bestSamplesSelection.cpp
  src/samplesSelection/randomSamplesSelection.cpp
  
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
  src/optimisationProblem/roboticsOptimisationProblem.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PRPR.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PRRR.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PRUS.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PUPS.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine4PRUS.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine6PRUS.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine6PUPS.cpp
  src/optimisationProblem/roboticsOptimisationProblem/robotModel/multiLevelStewartPlatform.cpp
  src/optimisationProblem/roboticsOptimisationProblem/endEffectorPoseError.cpp
  src/optimisationProblem/surrogateModel.cpp
  src/optimisationProblem/surrogateModel/polynomialFunctionModel.cpp
  src/optimisationProblem/surrogateModel/radialBasisFunctionModel.cpp
  src/optimisationProblem/surrogateModel/krigingModel.cpp
  src/optimisationProblem/surrogateModel/multivariateAdaptiveRegressionSplinesModel.cpp
  src/optimisationProblem/surrogateModel/artificialNeuralNetwork.cpp

  # Optimisation algorithms
  src/optimisationAlgorithm.cpp
  src/optimisationAlgorithm/samplingBasedOptimisationAlgorithm.cpp
  src/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/gridSearch.cpp
  src/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/randomSearch.cpp
  src/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.cpp
  src/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hillClimbing.cpp
  src/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hookeJeevesAlgorithm.cpp
  src/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/simulatedAnnealing.cpp
  src/optimisationAlgorithm/populationBasedOptimisationAlgorithm.cpp
  src/optimisationAlgorithm/populationBasedOptimisationAlgorithm/particleSwarmOptimisation.cpp
  
  # Property analyses
  src/propertyAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/fitnessDistanceCorrelationAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/lipschitzContinuityAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/hoelderContinuityAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/polynomialFunctionModelAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/additiveSeparabilityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/multiplicativeSeparabilityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/polynomialProportionalityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/neighbourhoodPeriodicityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/translationPeriodicityAnalysis.cpp
  
  # Cluster analyses
  src/clusterAnalysis.cpp
  src/clusterAnalysis/basicSequentialAlgorithmicScheme.cpp
  src/clusterAnalysis/twoThresholdSequentialAlgorithmicScheme.cpp
)

# Linking against prerequirements
if(ARMA_USE_WRAPPER)
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
  
  if(CMAKE_COMPILER_IS_CLANGXX OR CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "- Displaying compiler warnings.")
    if(CMAKE_COMPILER_IS_CLANGXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
    
    message(STATUS "- Handling warnings as errors.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()
  
  
  if(CMAKE_COMPILER_IS_CLANGXX)
    message(STATUS "- Including a undefined behavior checker and memory error detector.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --fsanitize=undefined -fsanitize=address")
  endif()
    
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
    test/catchExtension.cpp
    
    # # Configuration
    # test/testConfig.cpp
    
    # # Asserts
    # test/helper/testAssert.cpp

    # # Random number generator
    # test/helper/testRng.cpp
    
    # # MPI
    # test/helper/testMpi.cpp
    
    # # Helper
    # test/helper/testGeometry.cpp
    # test/helper/testPrintable.cpp
    # test/helper/testQuasiRandomSequence.cpp
    test/helper/testRandom.cpp
    test/helper/testRegression.cpp
    # test/helper/testSetTheory.cpp
    # test/helper/testStatistic.cpp
    # test/helper/testUnorderedContainer.cpp

    # # Distance functions
    # test/testDistanceFunction.cpp
    # test/distanceFunction/testPNorm.cpp
    # test/distanceFunction/testMahalabonisDistance.cpp
    
    # # Regression functions
    # test/testRegressionFunction.cpp
    # test/regressionFunction/testConstantPolynomialRegressionFunction.cpp
    # test/regressionFunction/testLinearPolynomialRegressionFunction.cpp
    # test/regressionFunction/testQuadraticPolynomialRegressionFunction.cpp
    # test/regressionFunction/testCubicPolynomialRegressionFunction.cpp

    # # Correlation functions
    # test/testCorrelationFunction.cpp
    # test/correlationFunction/testPolynomialCorrelationFunction.cpp
    # test/correlationFunction/testSplineCorrelationFunction.cpp
    # test/correlationFunction/testExponentialCorrelationFunction.cpp
    # test/correlationFunction/testGaussianCorrelationFunction.cpp
      
    # # Radial basis functions
    # test/testRadialBasisFunction.cpp
    # test/radialBasisFunction/testPolyharmonicSplineRadialBasisFunction.cpp
    # test/radialBasisFunction/testMultiquadraticRadialBasisFunction.cpp
    # test/radialBasisFunction/testGaussianRadialBasisFunction.cpp
      
    # # Samples selections
    # test/testSamplesSelection.cpp
    # test/samplesSelection/testBestFittingSamplesSelection.cpp
    # test/samplesSelection/testBestNeighbourhoodSamplesSelection.cpp
    # test/samplesSelection/testBestSamplesSelection.cpp
    # test/samplesSelection/testRandomSamplesSelection.cpp

    # # Optimisation problems
    # test/testOptimisationProblem.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testAttractiveSectorFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testBentCigarFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testBuecheRastriginFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testCompositeGriewankRosenbrockFunctionF8F2.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testDifferentPowersFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testDiscusFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testEllipsoidalFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testEllipsoidalFunctionRotated.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testGallaghersGaussian101mePeaksFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testGallaghersGaussian21hiPeaksFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testKatsuuraFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testLinearSlope.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testLunacekBiRastriginFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testRastriginFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testRastriginFunctionRotated.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testRosenbrockFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testRosenbrockFunctionRotated.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testSchaffersF7Function.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testSchaffersF7FunctionIllConditioned.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testSchwefelFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testSharpRidgeFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testSphereFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testStepEllipsoidalFunction.cpp
    # test/optimisationProblem/blackBoxOptimisationBenchmark/testWeierstrassFunction.cpp
    # test/optimisationProblem/testRoboticsOptimisationProblem.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/testRobotModel.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PRPR.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PRRR.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PRUS.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PUPS.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine4PRUS.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine6PRUS.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine6PUPS.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/robotModel/testMultiLevelStewartPlatform.cpp
    # test/optimisationProblem/roboticsOptimisationProblem/testEndEffectorPoseError.cpp
    # test/optimisationProblem/testSurrogateModel.cpp
    # test/optimisationProblem/surrogateModel/testPolynomialFunctionModel.cpp
    # test/optimisationProblem/surrogateModel/testRadialBasisFunctionModel.cpp
    # test/optimisationProblem/surrogateModel/testKrigingModel.cpp
    # test/optimisationProblem/surrogateModel/testMultivariateAdaptiveRegressionSplinesModel.cpp
    # test/optimisationProblem/surrogateModel/testArtificialNeuralNetwork.cpp
    
    # # Optimisation algorithms
    # test/testOptimisationAlgorithm.cpp
    # test/optimisationAlgorithm/testSamplingBasedOptimisationAlgorithm.cpp
    # test/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/testRandomSearch.cpp
    # test/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/testGridSearch.cpp
    # test/optimisationAlgorithm/testTrajectoryBasedOptimisationAlgorithm.cpp
    # test/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/testHillClimbing.cpp
    # test/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/testHookeJeevesAlgorithm.cpp
    # test/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/testSimulatedAnnealing.cpp
    # test/optimisationAlgorithm/testPopulationBasedOptimisationAlgorithm.cpp
    # test/optimisationAlgorithm/populationBasedOptimisationAlgorithm/testParticleSwarmOptimisation.cpp
    
    # # Samples selection
    # test/testSamplesSelection.cpp
    # test/samplesSelection/testBestFittingSamplesSelection.cpp
    # test/samplesSelection/testBestNeighbourhoodSamplesSelection.cpp
    # test/samplesSelection/testBestSamplesSelection.cpp
    # test/samplesSelection/testRandomSamplesSelection.cpp
    
    # # Property analysis
    # test/testPropertyAnalysis.cpp
    # test/propertyAnalysis/passivePropertyAnalysis/testFitnessDistanceCorrelationAnalysis.cpp
    # test/propertyAnalysis/passivePropertyAnalysis/testLipschitzContinuityAnalysis.cpp
    # test/propertyAnalysis/passivePropertyAnalysis/testHoelderContinuityAnalysis.cpp
    # test/propertyAnalysis/passivePropertyAnalysis/testPolynomialFunctionModelAnalysis.cpp
    # test/propertyAnalysis/testActivePropertyAnalysis.cpp
    # test/propertyAnalysis/activePropertyAnalysis/testAdditiveSeparabilityAnalysis.cpp
    # test/propertyAnalysis/activePropertyAnalysis/testMultiplicativeSeparabilityAnalysis.cpp
    # test/propertyAnalysis/activePropertyAnalysis/testPolynomialProportionalityAnalysis.cpp
    # test/propertyAnalysis/activePropertyAnalysis/testNeighbourhoodPeriodicityAnalysis.cpp
    # test/propertyAnalysis/activePropertyAnalysis/testTranslationPeriodicityAnalysis.cpp
  
    # # Cluster analyses
    # test/testClusterAnalysis.cpp
    # test/clusterAnalysis/testBasicSequentialAlgorithmicScheme.cpp
    # test/clusterAnalysis/testTwoThresholdSequentialAlgorithmicScheme.cpp
  )

  # Link against prerequirements
  target_link_libraries(mantellaTest mantella)
  
  if(ARMA_USE_WRAPPER)
    target_link_libraries(mantella ${ARMADILLO_LIBRARIES})
  else()
    target_link_libraries(mantella ${OPENBLAS_LIBRARIES})
  endif()

  if(SUPPORT_MPI)
    target_link_libraries(mantellaTest ${MPI_C_LIBRARIES})
  endif()

  if(SUPPORT_MPI)
    message(STATUS "- Performing tests with MPI")
    add_custom_command(TARGET mantellaTest POST_BUILD COMMAND mpiexec -n 4 ${EXECUTABLE_OUTPUT_PATH}/mantellaTest ${PROJECT_SOURCE_DIR}/test)
  else()
    add_custom_command(TARGET mantellaTest POST_BUILD COMMAND mantellaTest ${PROJECT_SOURCE_DIR}/test)
  endif()

  message(STATUS "- All tests will automatically be started after the library is build.")
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
