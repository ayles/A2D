cmake_minimum_required(VERSION 3.2)
project(Core CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../engine build_engine)

add_library(core
        src/root_component.h
        src/move_character.h
        src/move_camera.h
        src/root_component.cpp)

target_link_libraries(core a2d)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)