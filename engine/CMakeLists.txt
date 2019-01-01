cmake_minimum_required(VERSION 3.2)
project(A2D CXX)

set(CMAKE_CXX_STANDARD 11)

set(SHADERS_CREATE_IN_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/a2d/graphics/shaders_builtin.cpp.in")
set(SHADERS_CREATE_CPP_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/shaders_builtin.cpp")

set(A2D_SOURCES
        src/a2d/core/engine.cpp
        src/a2d/core/object2d.cpp
        src/a2d/core/component.cpp
        src/a2d/core/components/sprite.cpp
        src/a2d/graphics/renderer.cpp
        src/a2d/graphics/texture_region.cpp
        src/a2d/core/components/camera.cpp
        src/a2d/graphics/shader.cpp
        src/a2d/input/input.cpp
        src/a2d/core/components/pixel_camera.cpp
        src/a2d/core/sprite_batch.cpp
        src/a2d/core/bitmap_font.cpp
        src/a2d/core/drawable.cpp
        src/a2d/core/audio_clip.cpp
        src/a2d/core/audio.cpp
        src/a2d/core/components/audio_source.cpp
        src/a2d/core/components/text.cpp
        src/a2d/core/components/animator.cpp
        src/a2d/filesystem/filesystem.cpp
        src/a2d/graphics/texture_buffer.cpp
        src/a2d/graphics/texture.cpp
        ${SHADERS_CREATE_CPP_FILE_PATH})

set(A2D_HEADERS
        src/a2d/graphics/renderer.h
        src/a2d/a2d.h
        src/a2d/core/engine.h
        src/a2d/core/component.h
        src/a2d/core/object2d.h
        src/a2d/math/math.h
        src/a2d/math/vector.h
        src/a2d/math/matrix.h
        src/a2d/core/ref_counter.h
        src/a2d/core/intrusive_ptr.h
        src/a2d/core/macro.h
        src/a2d/core/components/sprite.h
        src/a2d/core/components/camera.h
        src/a2d/graphics/texture_region.h
        src/a2d/graphics/shader.h
        src/a2d/core/components/animator.h
        src/a2d/input/input.h
        src/a2d/core/components/events_test.h
        src/a2d/core/components/pixel_camera.h
        src/a2d/core/audio.h
        src/a2d/core/drawable.h
        src/a2d/core/sprite_batch.h
        src/a2d/core/components/text.h
        src/a2d/core/bitmap_font.h
        src/a2d/core/components/audio_source.h
        src/a2d/core/audio_clip.h
        src/a2d/core/physics.h
        src/a2d/core/components/physics/physics_body.h
        src/a2d/core/components/line.h
        src/a2d/core/components/physics/physics_collider.h
        src/a2d/core/components/physics/circle_collider.h
        src/a2d/core/gl.h
        src/a2d/filesystem/filesystem.h
        src/a2d/math/math_defines.h
        src/a2d/graphics/texture_buffer.h
        src/a2d/graphics/texture.h)

add_library(a2d STATIC ${A2D_SOURCES} ${A2D_HEADERS})
target_include_directories(a2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/lodepng)
add_subdirectory(thirdparty/simplejson)
add_subdirectory(thirdparty/freetype)
add_subdirectory(thirdparty/box2d)

set(SOLOUD_STATIC TRUE)
if (WIN32)
    set(SOLOUD_BACKEND_WINMM TRUE)
elseif (ANDROID)
    set(SOLOUD_BACKEND_OPENSLES TRUE)
elseif (UNIX)
    set(SOLOUD_BACKEND_ALSA TRUE)
endif ()

add_subdirectory(thirdparty/soloud)

target_link_libraries(a2d spdlog lodepng simplejson freetype soloud box2d)

if (ANDROID)
    target_compile_definitions(a2d PUBLIC RENDERER_GL_ES)
    target_compile_definitions(a2d PUBLIC TARGET_ANDROID)
    set(GLSL_LANG gles)
elseif (WIN32 OR UNIX OR APPLE)
    if (WIN32)
        target_compile_definitions(a2d PUBLIC TARGET_WINDOWS)
    elseif (UNIX)
        target_compile_definitions(a2d PUBLIC TARGET_LINUX)
    elseif (APPLE)
        target_compile_definitions(a2d PUBLIC TARGET_MACOS)
    endif ()

    find_package(OpenGL REQUIRED)

    target_compile_definitions(a2d PUBLIC RENDERER_GL)
    target_compile_definitions(a2d PUBLIC TARGET_DESKTOP)
    set(GLSL_LANG glsl)
    add_definitions(-DGLEW_STATIC)

    add_subdirectory(thirdparty/glew)
    add_subdirectory(thirdparty/glfw)

    target_link_libraries(a2d ${OPENGL_gl_LIBRARY} glew glfw)
endif ()

# Shaders building
file(WRITE ${SHADERS_CREATE_CPP_FILE_PATH} "")

add_subdirectory(thirdparty/glslcc)

add_custom_target(build_shaders ALL
        COMMAND ${CMAKE_COMMAND}
        -DSHADERS_CREATE_IN_FILE_PATH=${SHADERS_CREATE_IN_FILE_PATH}
        -DSHADERS_CREATE_CPP_FILE_PATH=${SHADERS_CREATE_CPP_FILE_PATH}
        -DSHADERS_PATH=${CMAKE_CURRENT_SOURCE_DIR}/src/a2d/graphics/shaders
        -DSHADERS_BUILD_PATH=${CMAKE_CURRENT_BINARY_DIR}/shaders
        -DGLSLCC_PATH=$<TARGET_FILE:glslcc>
        -DGLSL_LANG=${GLSL_LANG}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/src/a2d/graphics/build_shaders.cmake)

add_dependencies(build_shaders glslcc)
add_dependencies(a2d build_shaders)
