cmake_minimum_required(VERSION 3.2)
project(A2D)

set(CMAKE_CXX_STANDARD 11)

set(A2D_SOURCES
    src/math/vector2.cpp
    src/math/vector3.cpp
    src/math/vector4.cpp
    src/math/matrix4.cpp
    src/core/engine.cpp
    src/core/object2d.cpp
    src/core/component.cpp
    src/core/components/sprite.cpp
    src/core/renderer.cpp
    src/core/glsl_shader.cpp
    src/core/native_filesystem.cpp
    src/core/texture.cpp
)

set(A2D_HEADERS
    include/a2d/core/renderer.h
    include/a2d/core.h
    include/a2d/core/engine.h
    include/a2d/core/component.h
    include/a2d/core/object2d.h
    include/a2d/math.h
    include/a2d/math/vector.h
    include/a2d/math/matrix.h
    include/a2d/core/ref_counter.h
    include/a2d/core/intrusive_ptr.h
    include/a2d/core/macro.h
    include/a2d/core/components/sprite.h
    include/a2d/core/components/camera.h
    include/a2d/core/glsl_shader.h
    include/a2d/core/native_renderer.h
    include/a2d/core/filesystem.h
    include/a2d/core/native_filesystem.h
    include/a2d/core/texture.h
)

add_library(a2d STATIC ${A2D_SOURCES} ${A2D_HEADERS})
target_include_directories(a2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/lodepng)

target_link_libraries(a2d spdlog lodepng)

if (ANDROID)
    target_compile_definitions(a2d PUBLIC TARGET_ANDROID)
elseif(WIN32 OR LINUX OR APPLE)
    find_package(OpenGL REQUIRED)

    target_compile_definitions(a2d PUBLIC TARGET_DESKTOP)
    add_definitions(-DGLEW_STATIC)

    add_subdirectory(thirdparty/glew)
    add_subdirectory(thirdparty/glfw)
    add_subdirectory(thirdparty/zlib)
    set(PNG_BUILD_ZLIB TRUE)
    set(SKIP_INSTALL_ALL TRUE)
    set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zlib)
    set(ZLIB_LIBRARY zlibstatic)
    add_subdirectory(thirdparty/png)

    target_link_libraries(a2d ${OPENGL_gl_LIBRARY} glew glfw zlibstatic png_static)
endif()

add_subdirectory(thirdparty/glslcc)
add_dependencies(a2d glslcc)

#add_custom_target(process_shaders ALL COMMAND "$<TARGET_FILE:glslcc>")
#add_dependencies(process_shaders glslcc)
#add_dependencies(a2d process_shaders)

