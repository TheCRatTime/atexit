cmake_minimum_required(VERSION 3.10)

include(CTest)
include(CheckCCompilerFlag)

enable_testing()

add_executable(memory_test mem_test.c)
target_include_directories(memory_test PRIVATE ../include)
target_compile_options(memory_test PRIVATE -DATEXIT_DEBUG)
add_test(NAME memory.test COMMAND memory_test)
