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
    friend class NativeBridge;

    static int width;
    static int height;
    static pSpriteBatch sprite_batch;
#ifdef TARGET_DESKTOP
    static GLFWwindow *window;
#endif

public:
    enum ScreenOrientation {
        ORIENTATION_LANDSCAPE,
        ORIENTATION_REVERSE_LANDSCAPE,
        ORIENTATION_PORTRAIT,
        ORIENTATION_REVERSE_PORTRAIT
    };

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Renderer)
    Renderer() = delete;
    ~Renderer() = delete;

    static a2d::Vector4f clear_color;

    static int GetWidth();
    static int GetHeight();

    static pSpriteBatch GetSpriteBatch();
    static void SetSpriteBatch(const pSpriteBatch &sprite_batch);

    static const std::string &GetWindowTitle();
    static bool IsFullscreen();
    static ScreenOrientation GetScreenOrientation();

    /**
     * Sets windows title.
     * (Works only on desktop platforms)
     *
     * @param title
     */
    static void SetWindowTitle(const std::string &title);

    /**
     * Sets window resolution.
     * (Works only on desktop platforms)
     *
     * @param width
     * @param height
     */
    static void SetWindowResolution(int width, int height);

    /**
     * Sets fullscreen.
     * (Works only on desktop platforms)
     *
     * @param fullscreen is fullscreen should be set
     */
    static void SetFullscreen(bool fullscreen);

    /**
     * Sets screen orientation.
     * (Works only on mobile platforms)
     *
     * @param orientation screen orientation
     */
    static void SetScreenOrientation(ScreenOrientation orientation);

private:
    static bool Initialize();
    static bool Draw();
    static void Uninitialize();

    static void ResolutionChanged(int width, int height, int framebuffer_width = -1, int framebuffer_height = -1);
};

} //namespace a2d

#endif //A2D_RENDER_H
