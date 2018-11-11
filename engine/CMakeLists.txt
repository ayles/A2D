cmake_minimum_required(VERSION 3.2)
project(A2D)

set(CMAKE_CXX_STANDARD 11)

set(A2D_SOURCES
        src/math/vector2.cpp
        src/math/vector3.cpp
        src/math/vector4.cpp
        src/math/matrix4.cpp
        src/engine.cpp
        src/object2d.cpp
        src/component.cpp
        src/core/components/sprite.cpp
        src/renderer.cpp
        src/glsl_shader.cpp)

set(A2D_HEADERS
        include/A2D/renderer.h
        include/A2D/core.h
        include/A2D/core/engine.h
        include/A2D/core/component.h
        include/A2D/core/object2d.h
        include/A2D/math.h
        include/A2D/math/vector.h
        include/A2D/math/matrix.h
        include/A2D/core/ref_counter.h
        include/A2D/core/intrusive_ptr.h
        include/A2D/core/macro.h
        include/A2D/core/components/sprite.h
        include/A2D/core/components/camera.h
        include/A2D/glsl_shader.h)

add_library(a2d STATIC ${A2D_SOURCES} ${A2D_HEADERS})
target_include_directories(a2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(../project_core build_core)
target_link_libraries(a2d core)

if (ANDROID)

elseif(WIN32 OR LINUX OR APPLE)
    find_package(OpenGL REQUIRED)

    add_subdirectory(thirdparty/glew)
    add_subdirectory(thirdparty/glfw)
    add_subdirectory(thirdparty/lodepng)

    add_definitions(-DGLEW_STATIC)

    target_link_libraries(a2d ${OPENGL_gl_LIBRARY} glew glfw lodepng)
endif()
