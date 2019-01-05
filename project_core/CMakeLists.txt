cmake_minimum_required(VERSION 3.2)
project(Core CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../engine build_a2d_engine)

add_library(a2d_core_library
        src/root_component.hpp
        src/move_character.hpp
        src/move_camera.hpp
        src/root_component.cpp)

target_link_libraries(a2d_core_library a2d_engine_library)
target_include_directories(a2d_core_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)