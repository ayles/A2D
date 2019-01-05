//
// Created by selya on 26.10.2018.
//

#ifndef A2D_RENDER_H
#define A2D_RENDER_H

#include <a2d/core/macro.hpp>
#include <a2d/math/vector.hpp>
#include <a2d/graphics/sprite_batch.hpp>

#include <a2d/graphics/gl.hpp>

namespace a2d {

class Renderer {
    friend class Engine;
    friend class Sprite;
    friend class Input;
    friend class NativeConnector;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Renderer)
    Renderer() = delete;
    ~Renderer() = delete;

    static a2d::Vector4f clear_color;

    static int GetWidth();
    static int GetHeight();

    static pSpriteBatch GetSpriteBatch();
    static void SetSpriteBatch(const pSpriteBatch &sprite_batch);

private:
    static int width;
    static int height;
    static pSpriteBatch sprite_batch;
#ifdef TARGET_DESKTOP
    static GLFWwindow *window;
#endif

    static bool Initialize();
    static bool Draw();
    static void Uninitialize();

    static void ResolutionChanged(int width, int height, int framebuffer_width = -1, int framebuffer_height = -1);
};

} //namespace a2d

#endif //A2D_RENDER_H
