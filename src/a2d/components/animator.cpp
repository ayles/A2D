//
// Created by selya on 06.12.2018.
//

#include <a2d/components/animator.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {

Animator::Animator() : time(0.0f), playing(false), current_animation(nullptr) {}

void a2d::Animator::AddAnimation(const std::string &name, const pAnimation &animation) {
    animations[name] = animation;
}

void Animator::PlayAnimation(const std::string &name, bool reset) {
    auto a = animations.find(name);
    if (a != animations.end()) {
        if (current_animation != a->second || reset) time = 0.0f;
        current_animation = a->second;
        playing = true;
    } else {
        current_animation = nullptr;
        playing = false;
    }
}

void Animator::PauseAnimation() {
    playing = false;
}

void Animator::Update() {
    if (!current_animation) return;
    auto sprite = GetObject2D()->GetComponent<Sprite>();
    if (!sprite) return;
    auto frame = current_animation->GetFrame(time);
    if (!frame) return;
    sprite->SetTextureRegion(frame->texture_region);
    if (playing) time += a2d::Engine::GetDeltaTime();
}

Animator::~Animator() {}

} //namespace a2d
