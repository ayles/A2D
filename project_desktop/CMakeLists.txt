cmake_minimum_required(VERSION 3.2)
project(Desktop)

set(CMAKE_CXX_STANDARD 11)

set(CORE_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../project_core")

add_subdirectory(${CORE_PROJECT_PATH} build_core)

add_executable(desktop
    src/main.cpp
)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CORE_PROJECT_PATH}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_dependencies(desktop copy_resources)

target_link_libraries(desktop core)