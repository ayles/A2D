//
// Created by selya on 26.10.2018.
//

#ifndef A2D_RENDER_H
#define A2D_RENDER_H

#include <a2d/math.h>
#include <a2d/core/macro.h>
#include <a2d/core/engine.h>
#include <a2d/core/components/sprite.h>
#include <a2d/core/sprite_batch.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <set>
#include <stack>

namespace a2d {

class Renderer {
    friend class Engine;
    friend class Sprite;
    friend class NativeConnector;

public:
    Renderer() = delete;

#ifdef TARGET_DESKTOP
    static GLFWwindow *window;
#endif

    static GLuint vertex_buffer;

    static a2d::Vector4f clear_color;

    static int GetWidth();
    static int GetHeight();

private:
    static int width;
    static int height;
    static pSpriteBatch sprite_batch;

    static bool Initialize();
    static bool Draw();
    static void Uninitialize();

    static void ResolutionChanged(int width, int height);
};

} //namespace a2d

#endif //A2D_RENDER_H
