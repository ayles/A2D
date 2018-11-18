cmake_minimum_required(VERSION 3.2)
project(Core)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../engine build_engine)

add_library(core
    src/root_component.h
    src/root_component.cpp
        src/trump.h)

target_link_libraries(core a2d)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)