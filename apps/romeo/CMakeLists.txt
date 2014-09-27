cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Romeo's Master Thesis
project(RomeoMasterThesis CXX)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# ------------------------
# BUILD EVALUATION SCRIPTS
# ------------------------
message(STATUS "NOTICE: Configuring Romeo's master thesis evaluation scripts.")

# Include HOP headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate third party libraries and headers
find_package(Armadillo REQUIRED) # Armadillo C++
find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED) # Boost (system, filesystem)

# Include third party headers
include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++
include_directories(${Boost_INCLUDE_DIRS}) # Boost

# Compile the optimisation library
# All paths must start with "src/"
add_executable(parallisation
  main.cpp
)

# Link against HOP
target_link_libraries(parallisation "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libhop.so")

# Link against third party libraries
target_link_libraries(parallisation ${ARMADILLO_LIBRARIES}) # Armadillo C++
target_link_libraries(parallisation ${Boost_LIBRARIES}) # Boost
