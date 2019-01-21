//
// Created by selya on 14.11.2018.
//

#include <a2d/core/components/camera.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/a2d.hpp>

namespace a2d {

float Camera::GetHeight() {
    return height;
}

float Camera::GetWidth() {
    return height * GetAspectRatio();
}

float Camera::GetAspectRatio() {
    return (float)Renderer::GetWidth() / Renderer::GetHeight();
}

void Camera::SetHeight(float height) {
    this->height = height;
}

const Matrix4f &Camera::GetMatrix() {
    camera_transformed_matrix = camera_matrix * Matrix4f(GetObject2D()->GetTransformMatrix()).Inverse();
    return camera_transformed_matrix;
}

Camera &Camera::SetOrtho2D(float left, float right, float bottom, float top) {
    camera_matrix.SetOrtho2D(left, right, bottom, top);
    return *this;
}

Vector2f Camera::ScreenToWorld(const Vector2f &screen) {
    float x = screen.x / Renderer::GetWidth() * 2.0f  - 1.0f;
    float y = screen.y / Renderer::GetHeight() * 2.0f  - 1.0f;

    Vector4f v = camera_transformed_matrix.Unproject(x, y, 0);
    return Vector2f(v.x, v.y);
}

} //namespace a2d