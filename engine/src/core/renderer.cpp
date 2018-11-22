//
// Created by selya on 10.11.2018.
//

#include <a2d/core/renderer.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/camera.h>

a2d::Vector4f a2d::Renderer::clear_color = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
int a2d::Renderer::width = 1;
int a2d::Renderer::height = 1;
std::set<a2d::pSprite, a2d::Renderer::sprites_compare> a2d::Renderer::sprites;

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

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    bool first = true;
    for (const pSprite &sprite : sprites) {
        if (!sprite->IsActive() || !sprite->shader) continue;
        if (sprite->shader->Bind() || first) {
            sprite->shader->SetUniform("camera_matrix", a2d::Engine::camera->GetMatrix());
            first = false;
        }
        sprite->shader->SetUniform("model_matrix", sprite->GetObject2D()->GetTransformMatrix());
        sprite->shader->SetUniform("color", sprite->color);
        if (sprite->texture_region) {
            sprite->texture_region->Bind();
            //shader->Set("texture_sampler", 0);
            sprite->shader->SetUniform("uv_lb", sprite->texture_region->uv_lb);
            sprite->shader->SetUniform("uv_rt", sprite->texture_region->uv_rt);
            sprite->shader->SetUniform("ratio", sprite->texture_region->ratio);
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
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

void a2d::Renderer::AddSprite(const a2d::pSprite &sprite) {
    sprites.insert(sprite);
}

void a2d::Renderer::RemoveSprite(const a2d::pSprite &sprite) {
    sprites.erase(sprite);
}

bool a2d::Renderer::sprites_compare::operator()(const a2d::pSprite &lhs, const a2d::pSprite &rhs) const {
    std::stack<Object2D *> ls;
    std::stack<Object2D *> rs;
    ls.push(lhs->GetObject2D().get());
    rs.push(rhs->GetObject2D().get());
    while (ls.top()) {
        ls.push(ls.top()->GetParent().get());
    }
    while (rs.top()) {
        rs.push(rs.top()->GetParent().get());
    }
    while (true) {
        ls.pop();
        rs.pop();
        if (ls.top()->GetLayer() != rs.top()->GetLayer()) {
            return ls.top()->GetLayer() < rs.top()->GetLayer();
        } else if (ls.size() <= 1 || rs.size() <= 1) {
            // TODO compare by material
            return lhs < rhs;
        }
    }
}