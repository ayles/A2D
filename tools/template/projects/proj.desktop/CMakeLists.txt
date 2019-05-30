cmake_minimum_required(VERSION 3.2)
project("%A2D_PROJECT_NAME%")

set(CORE_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(EXECUTABLE_NAME ${PROJECT_NAME})
add_subdirectory(${CORE_PROJECT_PATH} a2d_core)
add_executable(${EXECUTABLE_NAME} src/main.cpp)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CORE_PROJECT_PATH}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)

add_dependencies(${EXECUTABLE_NAME} copy_resources)
target_link_libraries(${EXECUTABLE_NAME} a2d_core_library)