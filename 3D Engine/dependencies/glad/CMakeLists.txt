cmake_minimum_required(VERSION 3.12)

project(glad C)

set(GLAD_COMPILE_FLAGS "-w")

add_definitions(${GLAD_COMPILE_FLAGS})

add_library(glad
    "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h"
)
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
