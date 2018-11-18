//
// Created by selya on 14.11.2018.
//

#include <a2d/core/components/camera.h>
#include <a2d/core/object2d.h>
#include <a2d/core.h>

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
    camera_transformed_matrix = camera_matrix * GetObject2D()->GetTransformMatrix();
    return camera_transformed_matrix;
}

Camera &Camera::SetOrtho2D(float left, float right, float bottom, float top) {
    camera_matrix.SetOrtho2D(left, right, bottom, top);
    return *this;
}

} //namespace a2d