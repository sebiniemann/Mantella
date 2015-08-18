cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Mantella, C++ optimisation framework
project(Mantella CXX C)

# Retrieve the version numbers from "include/mantella"
file(STRINGS "${PROJECT_SOURCE_DIR}/include/mantella" MANTELLA_HEADER_CONTENTS REGEX "#define MANTELLA_VERSION_[A-Z]+ ")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MAJOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MAJOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_MINOR ([0-9]+).*" "\\1" MANTELLA_VERSION_MINOR "${MANTELLA_HEADER_CONTENTS}")
string(REGEX REPLACE ".*#define MANTELLA_VERSION_PATCH ([0-9]+).*" "\\1" MANTELLA_VERSION_PATCH "${MANTELLA_HEADER_CONTENTS}")
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

# Add additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

option(SUPPORT_SIMD "Add SSE2, SSE3, SSE4, AVX, etc. support" ON)
option(SUPPORT_MPI "Add MPI support" OFF)
# Development build options
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
  
find_package(Armadillo 5.100.2 REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIR})
  
if(SUPPORT_MPI)
  find_package(MPI REQUIRED)
  include_directories(${MPI_INCLUDE_PATH})
endif()

# ----------------------
# SETTING COMPILER FLAGS
# ----------------------

message(STATUS "") 
message(STATUS "Setting compiler flags.")

message(STATUS "- Enabling C++11 features.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message(STATUS "- Displaying all warnings.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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
  message(STATUS "- Using level 2 code optimisation.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
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
include_directories(${PROJECT_SOURCE_DIR}/include)

# All paths must start with "src/"
add_library(mantella SHARED
  src/config.cpp
  src/helper/assert.cpp

  # Random number generator
  src/helper/rng.cpp

  # Helper
  src/helper/geometry.cpp
  src/helper/printable.cpp
  src/helper/quasiRandomSequence.cpp
  src/helper/random.cpp
  src/helper/setTheory.cpp
  src/helper/unorderedContainer.cpp

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
    
  # Samples selection
  src/samplesSelection.cpp
  src/samplesSelection/bestFittingSamplesSelection.cpp
  src/samplesSelection/bestNeighbourhoodSamplesSelection.cpp
  src/samplesSelection/bestSamplesSelection.cpp
  src/samplesSelection/randomSamplesSelection.cpp

  # Property analyses
  src/propertyAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/fitnessDistanceCorrelationAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/lipschitzContinuityAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/hoelderContinuityAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/linearFunctionModelAnalysis.cpp
  src/propertyAnalysis/passivePropertyAnalysis/quadraticFunctionModelAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/additiveSeparabilityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/multiplicativeSeparabilityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/linearProportionalityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/neighbourhoodPeriodicityAnalysis.cpp
  src/propertyAnalysis/activePropertyAnalysis/translationPeriodicityAnalysis.cpp

  # Surrogate models
  src/surrogateModel.cpp
  src/surrogateModel/artificialNeuralNetwork.cpp
  src/surrogateModel/gaussianMixtureModel.cpp
  src/surrogateModel/krigingModel.cpp
  src/surrogateModel/linearFunctionModel.cpp
  src/surrogateModel/multivariateAdaptiveRegressionSplinesModel.cpp
  src/surrogateModel/quadraticFunctionModel.cpp
  src/surrogateModel/radialBasisFunctionModel.cpp
)

# Linking against prerequirements
target_link_libraries(mantella ${ARMADILLO_LIBRARIES})
if(SUPPORT_MPI)
  message(STATUS "- Adding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=Off' to exclude this.")
  target_link_libraries(mantella ${MPI_C_LIBRARIES})
else()
  message(STATUS "- Excluding MPI support.")
  message(STATUS "  - Use 'cmake ... -DSUPPORT_MPI=On' to add this.")
endif()

# -------------------
# BUILDING UNIT TESTS
# -------------------

if(BUILD_TESTS)
  message(STATUS "") 
  message(STATUS "Configuring unit tests.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=Off' to exclude this.")

  message(STATUS "- Forcing debug build for unit tests.")
  set(CMAKE_BUILD_TYPE "Debug")
  
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
    
    # Helper
    test/helper/testRng.cpp
    test/helper/testGeometry.cpp
    test/helper/testPrintable.cpp
    test/helper/testQuasiRandomSequence.cpp
    test/helper/testRandom.cpp
    test/helper/testSetTheory.cpp
    test/helper/testUnorderedContainer.cpp
    
    # Optimisation problems
    test/testOptimisationProblem.cpp
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
    test/optimisationProblem/testRoboticsOptimisationProblem.cpp
    test/optimisationProblem/roboticsOptimisationProblem/testRobotModel.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PRPR.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PRRR.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PRUS.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine3PUPS.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine4PRUS.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine6PRUS.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testParallelKinematicMachine6PUPS.cpp
    test/optimisationProblem/roboticsOptimisationProblem/robotModel/testMultiLevelStewartPlatform.cpp
    test/optimisationProblem/roboticsOptimisationProblem/testEndEffectorPoseError.cpp
    
    # Optimisation algorithms
    test/testOptimisationAlgorithm.cpp
    test/optimisationAlgorithm/testSamplingBasedOptimisationAlgorithm.cpp
    test/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/testRandomSearch.cpp
    test/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/testGridSearch.cpp
    test/optimisationAlgorithm/testTrajectoryBasedOptimisationAlgorithm.cpp
    test/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/testHillClimbing.cpp
    test/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/testHookeJeevesAlgorithm.cpp
    test/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/testSimulatedAnnealing.cpp
    test/optimisationAlgorithm/testPopulationBasedOptimisationAlgorithm.cpp
    test/optimisationAlgorithm/populationBasedOptimisationAlgorithm/testParticleSwarmOptimisation.cpp
    
    # Samples selection
    test/testSamplesSelection.cpp
    test/samplesSelection/testBestFittingSamplesSelection.cpp
    test/samplesSelection/testBestNeighbourhoodSamplesSelection.cpp
    test/samplesSelection/testBestSamplesSelection.cpp
    test/samplesSelection/testRandomSamplesSelection.cpp
    
    # Property analysis
    test/testPropertyAnalysis.cpp
    test/propertyAnalysis/passivePropertyAnalysis/testFitnessDistanceCorrelationAnalysis.cpp
    test/propertyAnalysis/passivePropertyAnalysis/testLipschitzContinuityAnalysis.cpp
    test/propertyAnalysis/passivePropertyAnalysis/testHoelderContinuityAnalysis.cpp
    test/propertyAnalysis/passivePropertyAnalysis/testLinearFunctionModelAnalysis.cpp
    test/propertyAnalysis/passivePropertyAnalysis/testQuadraticFunctionModelAnalysis.cpp
    test/propertyAnalysis/testActivePropertyAnalysis.cpp
    test/propertyAnalysis/activePropertyAnalysis/testAdditiveSeparabilityAnalysis.cpp
    test/propertyAnalysis/activePropertyAnalysis/testMultiplicativeSeparabilityAnalysis.cpp
    test/propertyAnalysis/activePropertyAnalysis/testLinearProportionalityAnalysis.cpp
    test/propertyAnalysis/activePropertyAnalysis/testNeighbourhoodPeriodicityAnalysis.cpp
    test/propertyAnalysis/activePropertyAnalysis/testTranslationPeriodicityAnalysis.cpp

    # Surrogate models
    test/testSurrogateModel.cpp
    test/surrogateModel/testArtificialNeuralNetwork.cpp
    test/surrogateModel/testGaussianMixtureModel.cpp
    test/surrogateModel/testKrigingModel.cpp
    test/surrogateModel/testLinearFunctionModel.cpp
    test/surrogateModel/testMultivariateAdaptiveRegressionSplinesModel.cpp
    test/surrogateModel/testQuadraticFunctionModel.cpp
    test/surrogateModel/testRadialBasisFunctionModel.cpp
  )

  # Link against prerequirements
  target_link_libraries(mantellaTest mantella)
  target_link_libraries(mantellaTest ${ARMADILLO_LIBRARIES})
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
message(STATUS "")

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