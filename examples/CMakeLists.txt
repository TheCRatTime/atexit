cmake_minimum_required(VERSION 3.10)
# include(CTest)
# include(CheckCCompilerFlag)

# enable_testing()

add_executable(ex1 ex1.c)
target_include_directories(ex1 PRIVATE ../include)
target_link_libraries(ex1 PRIVATE atexit_do)
