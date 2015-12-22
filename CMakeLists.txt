cmake_minimum_required(VERSION 3.3)
project(jitter)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES jitter.c)

add_executable(jitter ${SOURCE_FILES})
