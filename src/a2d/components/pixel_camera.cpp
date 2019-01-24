//
// Created by selya on 23.11.2018.
//

#include <a2d/components/pixel_camera.hpp>
#include <a2d/graphics/renderer.hpp>
#include <a2d/core/object2d.hpp>

a2d::PixelCamera::PixelCamera() : scale_factor(1) {}

void a2d::PixelCamera::SetScaleFactor(float scale_factor) {
    ASSERT_MAIN_THREAD
    this->scale_factor = scale_factor;
}

float a2d::PixelCamera::GetScaleFactor() {
    ASSERT_MAIN_THREAD
    return scale_factor;
}

void a2d::PixelCamera::Update() {
    ASSERT_MAIN_THREAD
    SetHeight((float)a2d::Renderer::GetHeight());
    Component::Update();
}

const a2d::Matrix4f &a2d::PixelCamera::GetMatrix() {
    ASSERT_MAIN_THREAD
    float half_height = GetHeight() * 0.5f * scale_factor;
    float half_width = GetAspectRatio() * half_height;
    SetOrtho2D(
            -half_width, half_width,
            -half_height, half_height
    );
    camera_transformed_matrix = camera_matrix * Matrix4f(GetObject2D()->GetTransformMatrix()).Inverse();
    return camera_transformed_matrix;
}
