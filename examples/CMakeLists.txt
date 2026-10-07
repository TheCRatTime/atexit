cmake_minimum_required(VERSION 3.10)

add_executable(ex1 ex1.c)
target_include_directories(ex1 PRIVATE ../include)
