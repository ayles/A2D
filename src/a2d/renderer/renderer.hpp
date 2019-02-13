//
// Created by selya on 26.10.2018.
//

#ifndef A2D_RENDERER_HPP
#define A2D_RENDERER_HPP

#include <a2d/renderer/gl.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/core/intrusive_ptr.hpp>

#include <set>
#include <string>

namespace a2d {

class Camera;

class Renderer {
    friend class Engine;
    friend class Input;
    friend class NativeBridge;

    static int width;
    static int height;
    static std::string window_title;
    static intrusive_ptr<Camera> main_camera;

#if TARGET_DESKTOP
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

    static int GetWidth();
    static int GetHeight();

    static intrusive_ptr<Camera> GetMainCamera();

    static const std::string &GetWindowTitle();
    static bool IsFullscreen();
    static ScreenOrientation GetScreenOrientation();

    static void SetMainCamera(const intrusive_ptr<Camera> &main_camera);

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

#endif //A2D_RENDERER_HPP
