cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Parallel optimisation applications
project(ParallelOptimisation CXX)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# -------------
# BUILD SCRIPTS
# -------------
message(STATUS "NOTICE: Configuring parallel optimisation applications.")

# Include Mantella headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate third party libraries and headers
find_package(Armadillo REQUIRED) # Armadillo C++
find_package(MPI REQUIRED) # MPI
find_package(Cereal REQUIRED) # Cereal (header only)

# Include third party headers
include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++
include_directories(${MPI_C_INCLUDE_PATH}) # MPI C-bindings
include_directories(${CEREAL_INCLUDE_DIRS}) # Cereal

# Compile the optimisation app
add_executable(standardParticleSwarmOptimisation2011ParameterStudy
  standardParticleSwarmOptimisation2011ParameterStudy.cpp
)

# Link against Mantella
target_link_libraries(standardParticleSwarmOptimisation2011ParameterStudy mantella)

# Link against third party libraries
target_link_libraries(standardParticleSwarmOptimisation2011ParameterStudy ${ARMADILLO_LIBRARIES}) # Armadillo C++
target_link_libraries(standardParticleSwarmOptimisation2011ParameterStudy ${MPI_C_LIBRARIES}) # MPI C-bindings
