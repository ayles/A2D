cmake_minimum_required(VERSION 3.2)
project("%A2D_PROJECT_NAME% core")

set(CMAKE_CXX_STANDARD 11)

add_subdirectory("%A2D_ENGINE_PATH%" a2d_engine)

add_library(a2d_core_library
        src/root_component.hpp src/root_component.cpp)

target_link_libraries(a2d_core_library a2d_engine_library)

# For root_component.hpp export
target_include_directories(a2d_core_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)