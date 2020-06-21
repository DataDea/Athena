cmake_minimum_required(VERSION 3.16)
project(athena C)

set(CMAKE_C_STANDARD 99)

ADD_SUBDIRECTORY(t_list)
ADD_SUBDIRECTORY(zmalloc)
SET(SOURCE_FILES main.c)
ADD_EXECUTABLE(athena ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(athena t_list)