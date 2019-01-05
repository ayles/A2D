cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

message(${CMAKE_C_FLAGS})

set(CORE_PROJECT_PATH "../../../../../project_core")

add_subdirectory(${CORE_PROJECT_PATH} build_a2d_core)

add_library(a2d_android_library SHARED jni_bindings.cpp)

# add lib dependencies
target_link_libraries(a2d_android_library
    android
    EGL
    GLESv2
    log
    a2d_core_library
)
