cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

message(${CMAKE_C_FLAGS})

add_subdirectory(../../../../../Engine engine_build)

add_library(a2d_android_native SHARED jni_bindings.cpp native_renderer.h native_renderer.cpp)

# add lib dependencies
target_link_libraries(a2d_android_native
    android
    EGL
    GLESv2
    log
    a2d
)
