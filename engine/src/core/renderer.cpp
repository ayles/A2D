//
// Created by selya on 10.11.2018.
//

#include <a2d/core/renderer.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/camera.h>

a2d::Vector4f a2d::Renderer::clear_color = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
int a2d::Renderer::width = 1;
int a2d::Renderer::height = 1;
a2d::pSpriteBatch a2d::Renderer::sprite_batch;

#if TARGET_DESKTOP
GLFWwindow *a2d::Renderer::window = nullptr;
#endif

GLuint a2d::Renderer::vertex_buffer = 0;

int a2d::Renderer::GetWidth() {
    return width;
}

int a2d::Renderer::GetHeight() {
    return height;
}

bool a2d::Renderer::Initialize() {
    static bool initialized = false;
    if (initialized) return initialized;

#ifdef TARGET_ANDROID
    a2d::Engine::GetLogger()->info("{} {}", "OpenGL ES version:", glGetString(GL_VERSION));
#elif TARGET_DESKTOP
    glfwSetErrorCallback([](int id, const char *description) {
        a2d::Engine::GetLogger()->error(description);
    });

    if (!glfwInit()) {
        a2d::Engine::GetLogger()->error("Couldn't init glfw");
        return initialized;
    }

    int width = 640;
    int height = 480;

    window = glfwCreateWindow(width, height, "a2d", nullptr, nullptr);
    if (!window) {
        a2d::Engine::GetLogger()->error("Couldn't create window");
        glfwTerminate();
        return initialized;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum glew_init_status = glewInit();

    if (glew_init_status != GLEW_OK) {
        a2d::Engine::GetLogger()->error(glewGetErrorString(glew_init_status));
        return initialized;
    }

    a2d::Engine::GetLogger()->info("{} {}", "OpenGL version:", glGetString(GL_VERSION));

    auto update_camera = [](GLFWwindow *window, int width, int height) {
        ResolutionChanged(width, height);
    };

    glfwSetFramebufferSizeCallback(window, update_camera);
    update_camera(window, width, height);

    auto window_focus = [](GLFWwindow *window, int focus) {
        if (focus) a2d::Engine::OnResume();
        else a2d::Engine::OnPause();
    };

    glfwSetWindowFocusCallback(window, window_focus);

#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sprite_batch = new SpriteBatch;

    return (initialized = true);
}

bool a2d::Renderer::Draw()  {
    if (!a2d::Engine::camera) return true;

    float half_height = a2d::Engine::camera->GetHeight() * 0.5f;
    float ratio = a2d::Engine::camera->GetAspectRatio();
    a2d::Engine::camera->SetOrtho2D(-half_height * ratio, half_height * ratio, -half_height,
                                    half_height);


#if TARGET_DESKTOP
    if (glfwWindowShouldClose(window))
        return false;
#endif

    glClearColor(
            a2d::Renderer::clear_color.x,
            a2d::Renderer::clear_color.y,
            a2d::Renderer::clear_color.z,
            a2d::Renderer::clear_color.w
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (a2d::Engine::camera) {
        sprite_batch->SetCameraMatrix(a2d::Engine::camera->GetMatrix());
        a2d::Engine::GetRoot()->Draw(*sprite_batch);
        sprite_batch->Flush();
    }

#if TARGET_DESKTOP
    glfwSwapBuffers(window);
    glfwPollEvents();
#endif
    return true;
}

void a2d::Renderer::Uninitialize() {
#if TARGET_DESKTOP
    glfwTerminate();
#endif
}

void a2d::Renderer::ResolutionChanged(int width, int height) {
    glViewport(0, 0, width, height);
    Renderer::width = width;
    Renderer::height = height;
}
