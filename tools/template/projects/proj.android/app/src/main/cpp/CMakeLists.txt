cmake_minimum_required(VERSION 3.4.1)
project("%A2D_PROJECT_NAME% android")

set(CORE_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../..")

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
