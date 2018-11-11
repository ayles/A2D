//
// Created by selya on 09.11.2018.
//

#ifndef A2D_NATIVE_RENDERER_H
#define A2D_NATIVE_RENDERER_H

#include <A2D/core.h>
#include <A2D/renderer.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "A2D/glsl_shader.h"

namespace a2d {

class NativeRenderer {
public:
    static GLFWwindow *window;
    static GLSLShader *shader;
    static GLuint vertex_buffer;

    static bool Initialize() {
        static bool initialized = false;
        if (initialized) return initialized;

        a2d::Renderer::Initialize();

        glfwSetErrorCallback([](int id, const char *description) {
            std::cerr << description << std::endl;
        });

        if (!glfwInit()) {
            std::cerr << "Couldn't init glfw" << std::endl;
            return initialized;
        }

        window = glfwCreateWindow(640, 480, "A2D", nullptr, nullptr);
        if (!window) {
            std::cerr << "Couldn't create window" << std::endl;
            glfwTerminate();
            return initialized;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        GLenum glew_init_status = glewInit();

        if (glew_init_status != GLEW_OK) {
            std::cerr << glewGetErrorString(glew_init_status) << std::endl;
            return initialized;
        }

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

        auto update_camera = [](GLFWwindow *window, int width, int height) {
            ResolutionChanged(width, height);
        };

        glfwSetFramebufferSizeCallback(window, update_camera);
        update_camera(window, 640, 480);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader = new GLSLShader("default",
            "#version 130\n"
            "\n"
            "uniform mat4 camera_matrix;\n"
            "uniform mat4 model_matrix;\n"
            "\n"
            "in vec2 position;\n"
            "\n"
            "out vec2 uv;\n"
            "\n"
            "void main() {\n"
            "    uv = position.xy / 2 + 0.5;\n"
            "    gl_Position = camera_matrix * model_matrix * vec4(position.x, position.y, 0, 1);\n"
            "}"
            "",

            "#version 130\n"
            "\n"
            "uniform sampler2D sprite_texture;\n"
            "uniform vec4 sprite_color;\n"
            "\n"
            "in vec2 uv;\n"
            "\n"
            "out vec4 out_color;\n"
            "\n"
            "void main() {\n"
            "    // Don't change mul order here\n"
            "    // Cause on my windows pc it will just show nothing\n"
            "    // But on Ubuntu WSL it works\n"
            "    // Maybe it is Nvidia driver bug\n"
            "    out_color = sprite_color;// * texture(sprite_texture, uv);\n"
            "}"
        );

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

        return (initialized = true);
    }

    static bool Draw() {
        if (glfwWindowShouldClose(window))
            return false;

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

        glfwSwapBuffers(window);
        glfwPollEvents();

        return true;
    }

    static void Uninitialize() {
        glfwTerminate();
        a2d::Renderer::Uninitialize();
    }

    static void ResolutionChanged(int width, int height) {
        glViewport(0, 0, width, height);
        a2d::Renderer::ResolutionChanged(width, height);
    }
};

GLFWwindow *a2d::NativeRenderer::window = nullptr;
GLSLShader *a2d::NativeRenderer::shader = nullptr;
GLuint a2d::NativeRenderer::vertex_buffer = 0;

} //namespace a2d

#endif //A2D_NATIVE_RENDERER_H
