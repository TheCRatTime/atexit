cmake_minimum_required(VERSION 3.10)
project(atexit C)
# include(CTest)
# include(CheckCCompilerFlag)

# enable_testing()

set(REAL_LIB_TYPE STATIC)
set(REAL_BUILD_EX OFF)

option(USE_SHARED "Use shared library instead of static" OFF)
option(BUILD_EX   "Build examples"                       OFF)

if(NOT USE_SHARED)
  message(STATUS "Using static library")
else()
  message(STATUS "Using shared library")
  set(REAL_LIB_TYPE SHARED)
endif()

if(NOT BUILD_EX)
  message(STATUS "Not building examples")
else()
  message(STATUS "Building examples")
  set(REAL_BUILD_EX ON)
endif()

add_library(atexit_do ${REAL_LIB_TYPE} src/atexit.c)
target_include_directories(atexit_do PRIVATE include)

if(REAL_BUILD_EX)
  add_subdirectory(examples)
endif()
