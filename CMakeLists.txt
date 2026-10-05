cmake_minimum_required(VERSION 3.10)

project(atexit C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)
include(CheckCCompilerFlag)

enable_testing()

# === PARSING USER VARIABLES ===
# == Setup 
set(REAL_LIB_TYPE    STATIC)
set(REAL_BUILD_EX       OFF)
set(REAL_USE_TESTS       ON)

# == Get options
option(USE_SHARED   "Use shared library instead of static"         OFF)
option(BUILD_EX     "Build examples"                               OFF)
option(USE_TESTS    "Build tests"                                   ON)

# == Library type
if(NOT USE_SHARED)
  message(STATUS "Using static library")
else()
  message(STATUS "Using shared library")
  set(REAL_LIB_TYPE SHARED)
endif()

# == Use tests
if(USE_TESTS)
  message(STATUS "Building tests")
else()
  message(STATUS "Not building tests")
  set(REAL_USE_TESTS OFF)
endif()

# == Build examples
if(NOT BUILD_EX)
  message(STATUS "Not building examples")
else()
  message(STATUS "Building examples")
  set(REAL_BUILD_EX ON)
endif()
################################

# Real library isn't for debug
add_library(atexit_do ${REAL_LIB_TYPE} src/atexit.c src/debug.c)

if(REAL_USE_TESTS)
  add_library(atexit_debug ${REAL_LIB_TYPE} src/atexit.c src/debug.c)
  target_compile_options(atexit_debug PRIVATE -DATEXIT_DEBUG -g)
  target_include_directories(atexit_debug PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
endif()

target_include_directories(atexit_do PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Build examples
if(REAL_BUILD_EX)
  add_subdirectory(examples)
endif()

# Build tests
if(REAL_USE_TESTS)
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)

install(TARGETS atexit_do
  EXPORT atexitTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Development
)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT Development
  FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY doc/man3/
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man3
  COMPONENT Documentation
  FILES_MATCHING PATTERN "*.3"
)

install(DIRECTORY doc/man3type/
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man3type
  COMPONENT Documentation
  FILES_MATCHING PATTERN "*.3type"
)
