//
// Created by selya on 14.11.2018.
//

#include <a2d/core/camera.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/renderer/renderer.hpp>
#include <a2d/renderer/framebuffer.hpp>
#include <a2d/renderer/sprite_batch.hpp>

namespace a2d {

float Camera::GetHeight() {
    return height;
}

float Camera::GetWidth() {
    return height * GetAspectRatio();
}

float Camera::GetAspectRatio() {
    if (framebuffer) return (float)framebuffer->GetWidth() / framebuffer->GetHeight();
    return (float)Renderer::GetWidth() / Renderer::GetHeight();
}

void Camera::SetHeight(float height) {
    this->height = height;
}

const Matrix4f &Camera::GetMatrix() {
    float half_height = GetHeight() * 0.5f;
    float ratio = GetAspectRatio();
    SetOrtho2D(
            -half_height * ratio, half_height * ratio,
            -half_height, half_height
    );
    return camera_matrix;
}

const Matrix4f &Camera::GetTransformedMatrix() {
    camera_transformed_matrix = GetMatrix() * Matrix4f(GetObject2D()->GetTransformMatrix()).Inverse();
    return camera_transformed_matrix;
}

const Vector4f &Camera::GetClearColor() {
    return clear_color;
}

intrusive_ptr<Framebuffer> Camera::GetFramebuffer() {
    return framebuffer;
}

void Camera::SetOrtho2D(float left, float right, float bottom, float top) {
    camera_matrix.SetOrtho2D(left, right, bottom, top);
}

void Camera::SetClearColor(float r, float g, float b, float a) {
    clear_color.Set(r, g, b, a);
}

void Camera::SetClearColor(const Vector4f &color) {
    SetClearColor(color.x, color.y, color.z, color.w);
}

void Camera::SetFramebuffer(const intrusive_ptr<Framebuffer> &framebuffer) {
    this->framebuffer = framebuffer;
}

Vector2f Camera::ScreenToWorld(const Vector2f &screen) {
    float x = screen.x / Renderer::GetWidth() * 2.0f  - 1.0f;
    float y = screen.y / Renderer::GetHeight() * 2.0f  - 1.0f;

    Vector4f v = camera_transformed_matrix.Unproject(x, y, 0);
    return Vector2f(v.x, v.y);
}

void Camera::Render() {
    ASSERT_MAIN_THREAD
    if (framebuffer) {
        framebuffer->Bind();
        glViewport(0, 0, framebuffer->GetWidth(), framebuffer->GetHeight());
    } else {
        glViewport(0, 0, Renderer::GetWidth(), Renderer::GetHeight());
    }

    Vector4f c = clear_color;
    auto o = GetObject2D();
    while (o) {
        c *= o->GetColor();
        o = o->GetParent();
    }

    glClearColor(c.x, c.y, c.z, c.w);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!sprite_batch) sprite_batch = new SpriteBatch;

    sprite_batch->SetCameraMatrix(GetTransformedMatrix());
    sprite_batch->Begin();
    Engine::GetRoot()->Draw(*sprite_batch);
    sprite_batch->End();

    /*static Matrix4f gui_camera_matrix;
    gui_camera_matrix.SetOrtho2D(0, Renderer::GetWidth(), 0, Renderer::GetHeight());
    sprite_batch->SetCameraMatrix(gui_camera_matrix);
    Engine::GetGUIRoot()->Draw(*sprite_batch);
    sprite_batch->Flush();*/

    if (framebuffer) Framebuffer::Unbind();
}

} //namespace a2d