//
// Created by selya on 19.11.2018.
//

#include <a2d/core/native_renderer.h>

#if TARGET_DESKTOP
GLFWwindow *a2d::NativeRenderer::window = nullptr;
#endif

GLuint a2d::NativeRenderer::vertex_buffer = 0;
