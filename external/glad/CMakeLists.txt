cmake_minimum_required(VERSION 3.10)
project(glad)

add_library(glad STATIC
    src/egl.c
    src/gl.c
)

target_include_directories(glad PUBLIC include)