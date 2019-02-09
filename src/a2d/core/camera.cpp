//
// Created by selya on 14.11.2018.
//

#include <a2d/core/camera.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/graphics/renderer.hpp>

namespace a2d {

float Camera::GetHeight() {
    ASSERT_MAIN_THREAD
    return height;
}

float Camera::GetWidth() {
    ASSERT_MAIN_THREAD
    return height * GetAspectRatio();
}

float Camera::GetAspectRatio() {
    ASSERT_MAIN_THREAD
    return (float)Renderer::GetWidth() / Renderer::GetHeight();
}

void Camera::SetHeight(float height) {
    ASSERT_MAIN_THREAD
    this->height = height;
}

const Matrix4f &Camera::GetMatrix() {
    ASSERT_MAIN_THREAD
    float half_height = GetHeight() * 0.5f;
    float ratio = GetAspectRatio();
    SetOrtho2D(
            -half_height * ratio, half_height * ratio,
            -half_height, half_height
    );
    return camera_matrix;
}

const Matrix4f &Camera::GetTransformedMatrix() {
    ASSERT_MAIN_THREAD
    camera_transformed_matrix = GetMatrix() * Matrix4f(GetObject2D()->GetTransformMatrix()).Inverse();
    return camera_transformed_matrix;
}

void Camera::SetOrtho2D(float left, float right, float bottom, float top) {
    ASSERT_MAIN_THREAD
    camera_matrix.SetOrtho2D(left, right, bottom, top);
}

Vector2f Camera::ScreenToWorld(const Vector2f &screen) {
    ASSERT_MAIN_THREAD
    float x = screen.x / Renderer::GetWidth() * 2.0f  - 1.0f;
    float y = screen.y / Renderer::GetHeight() * 2.0f  - 1.0f;

    Vector4f v = camera_transformed_matrix.Unproject(x, y, 0);
    return Vector2f(v.x, v.y);
}

} //namespace a2d