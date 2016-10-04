cmake_minimum_required(VERSION 3.6)
project(threads)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES src/main.c)
add_executable(threads ${SOURCE_FILES})