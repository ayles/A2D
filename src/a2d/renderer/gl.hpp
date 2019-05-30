//
// Created by selya on 27.12.2018.
//

#ifndef A2D_GL_H
#define A2D_GL_H

#if TARGET_ANDROID
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#elif TARGET_IOS
#include <OpenGLES/ES2/gl.h>
#elif TARGET_DESKTOP
#ifdef TARGET_WINDOWS
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif //A2D_GL_H
