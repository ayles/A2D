//
// Created by selya on 10.11.2018.
//

#include <a2d/renderer/renderer.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/camera.hpp>
#include <a2d/core/log.hpp>
#include <a2d/renderer/texture/texture_bind_manager.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/native_to_platform.h>

#if TARGET_ANDROID
#include <android/native_window_jni.h>
#endif

namespace a2d {

int Renderer::width = 1;
int Renderer::height = 1;
std::string Renderer::window_title;
intrusive_ptr<Camera> Renderer::main_camera;

#if TARGET_ANDROID
bool Renderer::window_changed = false;
EGLNativeWindowType Renderer::window = nullptr;
EGLSurface Renderer::surface = nullptr;
EGLDisplay Renderer::display = nullptr;
EGLContext Renderer::context = nullptr;
EGLConfig Renderer::config = nullptr;
#endif

#if TARGET_DESKTOP
GLFWwindow *Renderer::window = nullptr;
#endif

int Renderer::GetWidth() {
    return width;
}

int Renderer::GetHeight() {
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

#if TARGET_ANDROID
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    const EGLint context_attribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    EGLint numConfigs;
    EGLint format;

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        LOG_ERROR("eglGetDisplay() returned error %d", eglGetError());
        return false;
    }
    if (!eglInitialize(display, 0, 0)) {
        LOG_ERROR("eglInitialize() returned error %d", eglGetError());
        return false;
    }

    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
        LOG_ERROR("eglChooseConfig() returned error %d", eglGetError());
        return false;
    }

    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOG_ERROR("eglGetConfigAttrib() returned error %d", eglGetError());
        return false;
    }

    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

    if (!(context = eglCreateContext(display, config, 0, context_attribs))) {
        LOG_ERROR("eglCreateContext() returned error %d", eglGetError());
        return false;
    }

    if (!(surface = eglCreateWindowSurface(display, config, window, 0))) {
        LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
        return false;
    }

    int width, height;
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);
    ResolutionChanged(width, height);

    eglSwapInterval(display, 1);

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

#if TARGET_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

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
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);
    TextureBindManager::Initialize(max_texture_units);

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

#if TARGET_ANDROID
    if (window_changed || !surface) {
        window_changed = false;

        if (surface) {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            eglDestroySurface(display, surface);
            surface = nullptr;
        }

        if (!window) return true;
        if (!(surface = eglCreateWindowSurface(display, config, window, 0))) {
            LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
            return true;
        }

        if (!eglMakeCurrent(display, surface, surface, context)) {
            LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
            return true;
        }

        int width, height;
        eglQuerySurface(display, surface, EGL_WIDTH, &width);
        eglQuerySurface(display, surface, EGL_HEIGHT, &height);
        ResolutionChanged(width, height);
    }
#endif

#if TARGET_DESKTOP
    if (glfwWindowShouldClose(window))
        return false;
#endif

    if (main_camera) main_camera->Render();

#if TARGET_ANDROID
    if (!eglSwapBuffers(display, surface)) {
        window_changed = true;
        window = nullptr;
    }
#endif

#if TARGET_DESKTOP
    glfwSwapBuffers(window);
#endif
    return true;
}

void Renderer::Uninitialize() {
#if TARGET_ANDROID
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
#endif

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
    NativeToPlatform::SetOrientation(orientation);
}

} //namespace a2d
