cmake_minimum_required(VERSION 3.2)
project(Core)

set(CMAKE_CXX_STANDARD 11)

include_directories(../engine/include)

add_library(core
    src/root_component.h
    src/root_component.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)