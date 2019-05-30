//
// Created by selya on 06.03.2019.
//

#include <a2d/core/object2d.hpp>
#include <a2d/core/animation/animation.hpp>
#include <a2d/renderer/sprite.hpp>
#include <a2d/core/camera.hpp>
#include <cmath>

namespace a2d {

intrusive_ptr<Object2D> Animation::GetObject2D() {
    return object_2d;
}

Animations::ClearColorTo::ClearColorTo(const Vector4f &color) : color(color) {}

void Animations::ClearColorTo::Start() {
    color_from = GetObject2D()->GetComponent<Camera>()->GetClearColor();
}

void Animations::ClearColorTo::Step(float time) {
    GetObject2D()->GetComponent<Camera>()->SetClearColor(color_from.Lerp(color, time));
}

Animations::ColorTo::ColorTo(const Vector4f &color) : color(color) {}

void Animations::ColorTo::Start() {
    color_from = GetObject2D()->GetColor();
}

void Animations::ColorTo::Step(float time) {
    GetObject2D()->SetColor(color_from.Lerp(color, time));
}

Animations::Frames::Frames(const std::vector<intrusive_ptr<TextureRegion>> &frames) :
frames(frames) {}

void Animations::Frames::Start() {
    auto sprite = GetObject2D()->GetComponent<Sprite>();
    if (!sprite) return;
    sprite->SetTextureRegion(frames[0]);
}

void Animations::Frames::Step(float time) {
    auto sprite = GetObject2D()->GetComponent<Sprite>();
    if (!sprite) return;
    current_frame_index = int(time * frames.size());
    sprite->SetTextureRegion(frames[current_frame_index]);
}

int Animations::Frames::GetCurrentFrameIndex() {
    return current_frame_index;
}

}