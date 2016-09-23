cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(MANTELLA_VERSION_MAJOR 0)
set(MANTELLA_VERSION_MINOR 50)
set(MANTELLA_VERSION_PATCH 0)
set(MANTELLA_VERSION "${MANTELLA_VERSION_MAJOR}.${MANTELLA_VERSION_MINOR}.${MANTELLA_VERSION_PATCH}")

project(Mantella VERSION ${MANTELLA_VERSION} LANGUAGES CXX C)

# Adds additional cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Development build options
option(BUILD_TESTS "Build tests" OFF)
option(ADD_OPENMP_TESTS "Add OpenMP tests" OFF)
option(ADD_MPI_TESTS "Add MPI tests" OFF)

mark_as_advanced(
  BUILD_TESTS
  ADD_OPENMP_TESTS
  ADD_MPI_TESTS
)

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endif()

if (NOT INSTALL_INCLUDE_DIR)
  set(INSTALL_INCLUDE_DIR "include")
endif()

message(STATUS "") 
message(STATUS "Preparing Mantella (version: ${MANTELLA_VERSION}).")

# -------------------
# CONFIGURING LIBRARY
# -------------------

message(STATUS "") 
message(STATUS "Configuring library.")

configure_file(${PROJECT_SOURCE_DIR}/include/mantella.cmake ${PROJECT_SOURCE_DIR}/include/mantella)

# ----------------------------
# CONFIGURING UNIT TESTS BUILD
# ----------------------------

if (BUILD_TESTS)
  message(STATUS "") 
  message(STATUS "Configuring unit tests build.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=OFF' to exclude this.")
  
  find_package(Catch REQUIRED)
  
  if (ADD_OPENMP_TESTS)
    find_package(OpenMP REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  else()
  
  if (ADD_MPI_TESTS)
    find_package(MPI REQUIRED)
  else()
  
  add_executable(mantellaTests test/main.cpp)
  set_property(TARGET mantellaTests PROPERTY CXX_STANDARD 14)
  set_property(TARGET mantellaTests PROPERTY CXX_STANDARD_REQUIRED ON)

  message(STATUS "- Adding debugging symbols.")
  target_compile_options(mantellaTests PRIVATE -g)
  
  if (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "- Adding code coverage measurement.")
    target_compile_options(mantella PRIVATE --coverage)
    target_link_libraries(mantella PRIVATE --coverage)
  endif()
  
  message(STATUS "- Displaying compiler warnings (globally).")
  if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(
      -Wall
      -Wextra
      -pedantic-errors
      -Wno-unknown-pragmas)
  else()
    add_compile_options(
      -Weverything
      -fsanitize=undefined
      -Wno-c++98-compat
      -Wno-weak-vtables
      -Wno-exit-time-destructors
      -Wno-global-constructors
      -Wno-unknown-warning-option
      -Wno-source-uses-openmp)
  endif()
  
  message(STATUS "- Handling warnings as errors (globally).")
  add_compile_options(
    -Wno-padded
    -Wno-unused-parameter
    -Wno-c++98-compat-pedantic
    -Werror)
  
  target_include_directories(mantellaTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_include_directories(mantellaTests PRIVATE ${CATCH_INCLUDE_DIRS})
  if (SUPPORT_MPI)
    target_include_directories(mantellaTests SYSTEM PRIVATE ${MPI_C_INCLUDE_PATH})
  endif()
  
  target_link_libraries(mantellaTests PRIVATE ${BLAS_LIBRARIES})
  target_link_libraries(mantellaTests PRIVATE ${LAPACK_LIBRARIES})
  if (SUPPORT_MPI)
    target_link_libraries(mantellaTests PRIVATE ${MPI_C_LIBRARIES})
  endif()
else()
  message(STATUS "") 
  message(STATUS "Skipping unit tests build.")
  message(STATUS "- Use 'cmake ... -DBUILD_TESTS=ON' to add this.")
endif()

# --------------------------------
# CONFIGURING LIBRARY INSTALLATION
# --------------------------------

message(STATUS "")
message(STATUS "Configuring library installation.")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/mantella_bits/ DESTINATION ${INSTALL_INCLUDE_DIR}/mantella${MANTELLA_VERSION_MAJOR}_bits/ COMPONENT dev)
install(FILES ${PROJECT_SOURCE_DIR}/include/mantella DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev RENAME mantella${MANTELLA_VERSION_MAJOR})

# ---------------------------------
# PRINTING SOME NOTICABLE VARIABLES
# ---------------------------------

message(STATUS "")
message(STATUS "Noticable variables:")
message(STATUS "- CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "- CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message(STATUS "- CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "- CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "- SUPPORT_OPENMP = ${SUPPORT_OPENMP}")
message(STATUS "- SUPPORT_MPI = ${SUPPORT_MPI}")
message(STATUS "- INSTALL_INCLUDE_DIR = ${INSTALL_INCLUDE_DIR}")
message(STATUS "- INSTALL_LIB_DIR = ${INSTALL_LIB_DIR}")
message(STATUS "- INSTALL_MEX_FILES_DIR = ${INSTALL_MEX_FILES_DIR}")
message(STATUS "")
