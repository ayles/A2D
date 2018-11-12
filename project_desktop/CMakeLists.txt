cmake_minimum_required(VERSION 3.2)
project(Desktop)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../project_core build_core)

add_executable(desktop
    src/main.cpp
    src/native_renderer.cpp
    src/native_renderer.h
)

target_link_libraries(desktop core)