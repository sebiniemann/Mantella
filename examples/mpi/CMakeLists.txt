cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Experiemental MPI applications
project(ExperiementalMpi CXX)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# -------------
# BUILD SCRIPTS
# -------------
message(STATUS "NOTICE: Configuring experiemental MPI applications.")

# Include Mantella headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate third party libraries and headers
find_package(Armadillo 4.100.0 REQUIRED) # Armadillo C++
find_package(MPI 3.0.0 REQUIRED) # MPI

# Include third party headers
include_directories(${ARMADILLO_INCLUDE_DIR}) # Armadillo C++
include_directories(${MPI_C_INCLUDE_PATH}) # MPI C-bindings

# Compile the optimisation app
add_executable(parallelSPSO11
  parallelSPSO11.cpp
)

# Link against third party libraries
target_link_libraries(parallelSPSO11 ${ARMADILLO_LIBRARIES}) # Armadillo C++
target_link_libraries(parallelSPSO11 ${MPI_C_LIBRARIES}) # MPI C-bindings
