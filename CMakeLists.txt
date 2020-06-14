cmake_minimum_required(VERSION 3.16)
project(athena C)

set(CMAKE_C_STANDARD 99)

add_executable(athena main.c t_list/list.h t_list/list.c zmalloc.cpp t_list/test_list/test_list.cpp)