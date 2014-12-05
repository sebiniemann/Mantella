cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# Experiemental online optimisation applications
project(ExperiementalOnlineOptimisation CXX)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# -------------
# BUILD SCRIPTS
# -------------
message(STATUS "NOTICE: Configuring experiemental online optimisation applications.")

# Include HOP headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate third party libraries and headers
find_package(Armadillo REQUIRED) # Armadillo C++

# Include third party headers
include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++

# Compile the app
add_executable(exampleOnlineOptimisation
  main.cpp
)

# Link against HOP
target_link_libraries(exampleOnlineOptimisation hop)

# Link against third party libraries
target_link_libraries(exampleOnlineOptimisation ${ARMADILLO_LIBRARIES}) # Armadillo C++
