cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Experiemental robotic applications
project(ExperiementalRobotic CXX)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# -------------
# BUILD SCRIPTS
# -------------
message(STATUS "NOTICE: Configuring experiemental robotic applications.")

# Include own headers
include_directories(./)

# Include HOP headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate third party libraries and headers
find_package(Armadillo REQUIRED) # Armadillo C++

# Include third party headers
include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++

# Compile the optimisation library
# All paths must start with "src/"
add_executable(robotic
  robotic.cpp
  parallelKinematicMachine_3prrr_intersectionBased.cpp
  parallelKinematicMachine_3prrr_trigonometryBased.cpp
  parallelKinematicMachine_6prus_intersectionBased.cpp
  parallelKinematicMachine_6prus_trigonometryBased.cpp
)

# Link against HOP
target_link_libraries(robotic "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libhop.so")

# Link against third party libraries
target_link_libraries(robotic ${ARMADILLO_LIBRARIES}) # Armadillo C++
