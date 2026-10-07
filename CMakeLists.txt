cmake_minimum_required(VERSION 3.10)

project(atexit C)

include(CTest)
include(CheckCCompilerFlag)

enable_testing()

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(REAL_USE_TESTS       ON)

# == Get options
option(USE_TESTS "Build tests" ON)

# == Use tests
if(USE_TESTS)
  message(STATUS "Building tests")
else()
  message(STATUS "Not building tests")
  set(REAL_USE_TESTS OFF)
endif()

message(STATUS "Building examples")

add_subdirectory(examples)

if(REAL_USE_TESTS)
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)

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

install(DIRECTORY doc/man3head/
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man3head
  COMPONENT Documentation
  FILES_MATCHING PATTERN "*.3head"
)
