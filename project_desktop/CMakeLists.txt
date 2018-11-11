cmake_minimum_required(VERSION 3.2)
project(Desktop)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../engine engine_build)

add_executable(desktop
    src/main.cpp
    src/native_renderer.cpp
    src/native_renderer.h
)

target_link_libraries(desktop a2d)