//
// Created by selya on 27.12.2018.
//

#ifndef A2D_GL_H
#define A2D_GL_H

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif //A2D_GL_H
