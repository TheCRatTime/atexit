cmake_minimum_required(VERSION 3.10)
include(CTest)
include(CheckCCompilerFlag)

enable_testing()

add_executable(memory_test mem_test.c)
target_compile_options(memory_test PRIVATE -DATEXIT_DEBUG)
target_link_libraries(memory_test PRIVATE atexit_debug)
add_test(NAME memory.test COMMAND memory_test)
