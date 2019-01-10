//
// Created by selya on 10.11.2018.
//

#include <a2d/graphics/renderer.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/components/camera.hpp>

#include <a2d/graphics/gl.hpp>

namespace a2d {

Vector4f Renderer::clear_color = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
int Renderer::width = 1;
int Renderer::height = 1;
pSpriteBatch Renderer::sprite_batch;

#if TARGET_DESKTOP
GLFWwindow *Renderer::window = nullptr;
#endif

int Renderer::GetWidth() {
    return width;
}

int Renderer::GetHeight() {
    return height;
}

pSpriteBatch Renderer::GetSpriteBatch() {
    return sprite_batch;
}

void Renderer::SetSpriteBatch(const pSpriteBatch &sprite_batch) {
    Renderer::sprite_batch = sprite_batch;
}

bool Renderer::Initialize() {
    static bool initialized = false;
    if (initialized) return initialized;

#ifdef TARGET_MOBILE
    Engine::GetLogger()->info("{} {}", "OpenGL ES version:", glGetString(GL_VERSION));
#elif TARGET_DESKTOP
    glfwSetErrorCallback([](int id, const char *description) {
        Engine::GetLogger()->error(description);
    });

    if (!glfwInit()) {
        Engine::GetLogger()->error("Couldn't init glfw");
        return initialized;
    }

    int width = 640;
    int height = 480;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "a2d", nullptr, nullptr);
    if (!window) {
        Engine::GetLogger()->error("Couldn't create window");
        glfwTerminate();
        return initialized;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum glew_init_status = glewInit();

    if (glew_init_status != GLEW_OK) {
        Engine::GetLogger()->error(glewGetErrorString(glew_init_status));
        return initialized;
    }

    Engine::GetLogger()->info("{} {}", "OpenGL version:", glGetString(GL_VERSION));

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    SetSpriteBatch(new SpriteBatch);

    Shader::CreateDefaultShaders();

    return (initialized = true);
}

bool Renderer::Draw() {
    if (!Engine::camera) return true;

    float half_height = Engine::camera->GetHeight() * 0.5f;
    float ratio = Engine::camera->GetAspectRatio();
    Engine::camera->SetOrtho2D(
            -half_height * ratio, half_height * ratio,
            -half_height, half_height
    );


#if TARGET_DESKTOP
    if (glfwWindowShouldClose(window))
        return false;
#endif

    glClearColor(
            Renderer::clear_color.x,
            Renderer::clear_color.y,
            Renderer::clear_color.z,
            Renderer::clear_color.w
    );

    glClear(GL_COLOR_BUFFER_BIT);

    if (Engine::camera) {
        sprite_batch->SetCameraMatrix(Engine::camera->GetMatrix());
        Engine::GetRoot()->Draw(Matrix4f(), *sprite_batch);
        sprite_batch->Flush();
    }

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
#ifdef TARGET_DESKTOP
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

} //namespace a2d
