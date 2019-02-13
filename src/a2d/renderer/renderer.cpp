//
// Created by selya on 10.11.2018.
//

#include <a2d/renderer/renderer.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/camera.hpp>
#include <a2d/core/log.hpp>

#if TARGET_ANDROID
void Android_setOrientation(int orientation);
#endif

namespace a2d {

int Renderer::width = 1;
int Renderer::height = 1;
std::string Renderer::window_title;
intrusive_ptr<Camera> Renderer::main_camera;

#if TARGET_DESKTOP
GLFWwindow *Renderer::window = nullptr;
#endif

int Renderer::GetWidth() {
    ASSERT_MAIN_THREAD
    return width;
}

int Renderer::GetHeight() {
    ASSERT_MAIN_THREAD
    return height;
}

intrusive_ptr<Camera> Renderer::GetMainCamera() {
    return main_camera;
}

void Renderer::SetMainCamera(const intrusive_ptr<Camera> &main_camera) {
    Renderer::main_camera = main_camera;
}

bool Renderer::Initialize() {
    static bool initialized = false;
    if (initialized) return initialized;

#if TARGET_MOBILE
    Logger::Info("{} {}", "OpenGL ES version:", glGetString(GL_VERSION));
#elif TARGET_DESKTOP
    glfwSetErrorCallback([](int id, const char *description) {
        Logger::Error(description);
    });

    if (!glfwInit()) {
        Logger::Error("Couldn't init glfw");
        return initialized;
    }

    int width = 640;
    int height = 480;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, "a2d", nullptr, nullptr);
    if (!window) {
        Logger::Error("Couldn't create window");
        glfwTerminate();
        return initialized;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum glew_init_status = glewInit();

    if (glew_init_status != GLEW_OK) {
        Logger::Error((char *)glewGetErrorString(glew_init_status));
        return initialized;
    }

    Logger::Info("{} {}", "OpenGL version:", glGetString(GL_VERSION));

    auto update_camera = [](GLFWwindow *window, int width, int height) {
        ResolutionChanged(width, height);
    };

    glfwSetWindowSizeCallback(window, update_camera);
    update_camera(window, width, height);

    auto window_focus = [](GLFWwindow *window, int focus) {
        if (focus) Engine::Resume();
        else Engine::Pause();
    };

    glfwSetWindowFocusCallback(window, window_focus);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif

    GLint max_texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    Texture::unit_to_handle.resize(max_texture_units);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

#if RENDERER_GL
    //glEnable(GL_MULTISAMPLE);
#endif

    Shader::CreateDefaultShaders();

    return (initialized = true);
}

bool Renderer::Draw() {
    if (!main_camera) return true;

#if TARGET_DESKTOP
    if (glfwWindowShouldClose(window))
        return false;
#endif

    if (main_camera) main_camera->Render();

#if TARGET_DESKTOP
    glfwSwapBuffers(window);
#endif
    return true;
}

void Renderer::Uninitialize() {
#if TARGET_DESKTOP
    glfwTerminate();
#endif
}

void Renderer::ResolutionChanged(int width, int height, int framebuffer_width, int framebuffer_height) {
    if (width == Renderer::width && height == Renderer::height) return;
    // On apple retina framebuffer can differ from window size
    Renderer::width = width;
    Renderer::height = height;
#if TARGET_DESKTOP
    int f_w, f_h;
    glfwGetFramebufferSize(window, &f_w, &f_h);
    if (framebuffer_width < 0) framebuffer_width = f_w;
    if (framebuffer_height < 0) framebuffer_height = f_h;
#elif TARGET_MOBILE
    if (framebuffer_width < 0) framebuffer_width = width;
    if (framebuffer_height < 0) framebuffer_height = height;
#endif
    glViewport(0, 0, framebuffer_width, framebuffer_height);
}

void Renderer::SetScreenOrientation(Renderer::ScreenOrientation orientation) {
#if TARGET_ANDROID
    int orientation_native = 0;
    switch (orientation) {
        case ScreenOrientation::ORIENTATION_LANDSCAPE:
            orientation_native = 0;
            break;

        case ScreenOrientation::ORIENTATION_PORTRAIT:
            orientation_native = 1;
            break;

        case ScreenOrientation::ORIENTATION_REVERSE_LANDSCAPE:
            orientation_native = 8;
            break;

        case ScreenOrientation::ORIENTATION_REVERSE_PORTRAIT:
            orientation_native = 9;
            break;
    }
    Android_setOrientation(orientation_native);
#endif
}

} //namespace a2d
