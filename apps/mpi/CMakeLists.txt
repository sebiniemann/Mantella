#cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

## Experiemental MPI applications
#project(ExperiementalMpi CXX)

## Enable C++11 features
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

## Set output directories
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## -------------
## BUILD SCRIPTS
## -------------
#message(STATUS "NOTICE: Configuring experiemental MPI applications.")

## Include HOP headers
#include_directories(${PROJECT_SOURCE_DIR}/include)

## Locate third party libraries and headers
#find_package(Armadillo REQUIRED) # Armadillo C++
#find_package(MPI REQUIRED) # MPI

## Include third party headers
#include_directories(${ARMADILLO_INCLUDE_DIRS}) # Armadillo C++
#include_directories(${MPI_C_INCLUDE_PATH}) # MPI C-bindings

## Compile the optimisation library
## All paths must start with "src/"
#add_executable(parallelSPSO11
#  parallelSPSO11.cpp
#)

## Link against HOP
#target_link_libraries(parallelSPSO11 "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libhop.so")

## Link against third party libraries
#target_link_libraries(parallelSPSO11 ${ARMADILLO_LIBRARIES}) # Armadillo C++
#target_link_libraries(parallelSPSO11 ${MPI_C_LIBRARIES}) # MPI C-bindings
