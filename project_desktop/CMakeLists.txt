cmake_minimum_required(VERSION 3.2)
project(Desktop)

set(CMAKE_CXX_STANDARD 11)

set(CORE_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../project_core")

add_subdirectory(${CORE_PROJECT_PATH} build_a2d_core)

add_executable(A2D_Desktop
    src/main.cpp
)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CORE_PROJECT_PATH}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_dependencies(A2D_Desktop copy_resources)

target_link_libraries(A2D_Desktop a2d_core_library)