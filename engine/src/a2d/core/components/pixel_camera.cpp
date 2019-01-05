//
// Created by selya on 23.11.2018.
//

#include <a2d/core/components/pixel_camera.hpp>
#include <a2d/graphics/renderer.hpp>


a2d::PixelCamera::PixelCamera() : scale_factor(1) {

}

void a2d::PixelCamera::SetScaleFactor(float scale_factor) {
    this->scale_factor = scale_factor;
}

float a2d::PixelCamera::GetScaleFactor() {
    return scale_factor;
}

void a2d::PixelCamera::Update() {
    SetHeight((float)a2d::Renderer::GetHeight());
    Component::Update();
}
