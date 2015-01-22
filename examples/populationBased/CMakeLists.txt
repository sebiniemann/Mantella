cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Experiemental online optimisation applications
project(populationBasedOnlineOptimisation CXX)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# -------------
# BUILD SCRIPTS
# -------------
message(STATUS "NOTICE: Configuring populationbased online optimisation applications.")

# Include Mantella headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate third party libraries and headers
find_package(Armadillo REQUIRED) # Armadillo C++

# Include third party headers
include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++

# Compile the optimisation app
add_executable(CMAES
  CMAES.cpp
)

# Link against third party libraries
target_link_libraries(CMAES ${ARMADILLO_LIBRARIES}) # Armadillo C++
