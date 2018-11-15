//
// Created by selya on 09.11.2018.
//

#ifndef A2D_NATIVE_RENDERER_H
#define A2D_NATIVE_RENDERER_H

#include <a2d/core.h>
#include <a2d/core/glsl_shader.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <iostream>

namespace a2d {

class NativeRenderer {
public:

#ifdef TARGET_DESKTOP
    static GLFWwindow *window;
#endif

    static GLSLShader *shader;
    static GLuint vertex_buffer;

    static bool Initialize() {
        static bool initialized = false;
        if (initialized) return initialized;

        a2d::Renderer::Initialize();
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

        window = glfwCreateWindow(640, 480, "a2d", nullptr, nullptr);
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
        update_camera(window, 640, 480);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        static const GLfloat g_vertex_buffer_data[] = {
                -0.5f, -0.5f,
                0.5f, 0.5f,
                -0.5f, 0.5f,
                -0.5f, -0.5f,
                0.5f, -0.5f,
                0.5f,  0.5f,
        };

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        shader = new GLSLShader("default", FileSystem::LoadText("shaders/default/vertex.glsl"), FileSystem::LoadText("shaders/default/fragment.glsl"));

        return (initialized = true);
    }

    static bool Draw() {
#if TARGET_DESKTOP
        if (glfwWindowShouldClose(window))
            return false;
#endif
        if (!a2d::Renderer::PrepareDraw()) return false;

        glClearColor(a2d::Renderer::clear_color.x, a2d::Renderer::clear_color.y, a2d::Renderer::clear_color.z, a2d::Renderer::clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw here
        shader->Bind();
        shader->SetUniformValue("camera_matrix", a2d::Engine::camera->GetMatrix());
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        for (const pSprite &sprite : a2d::Renderer::sprites) {
            if (!sprite->IsActive()) continue;
            shader->SetUniformValue("model_matrix", sprite->GetObject2D()->GetTransformMatrix());
            shader->SetUniformValue("sprite_color", sprite->color);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

#if TARGET_DESKTOP
        glfwSwapBuffers(window);
        glfwPollEvents();
#endif
        return true;
    }

    static void Uninitialize() {
#if TARGET_DESKTOP
        glfwTerminate();
#endif
        a2d::Renderer::Uninitialize();
    }

    static void ResolutionChanged(int width, int height) {
        glViewport(0, 0, width, height);
        a2d::Renderer::ResolutionChanged(width, height);
    }
};

#if TARGET_DESKTOP
GLFWwindow *a2d::NativeRenderer::window = nullptr;
#endif

GLSLShader *a2d::NativeRenderer::shader = nullptr;
GLuint a2d::NativeRenderer::vertex_buffer = 0;

} //namespace a2d

#endif //A2D_NATIVE_RENDERER_H
