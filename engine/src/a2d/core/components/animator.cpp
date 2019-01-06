//
// Created by selya on 06.12.2018.
//

#include <a2d/core/components/animator.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {


Animation::Frame::Frame(const pTextureRegion &texture_region, float time) : texture_region(texture_region), time(time) {}

Animation::Animation(std::vector<Animation::Frame> frames) : elapsed_frame_time(0), current_frame_index(0), frames(std::move(frames)) {}

const Animation::Frame *Animation::GetCurrentFrame() const {
    if (frames.empty()) return nullptr;
    return &frames[current_frame_index];
}

void Animation::AddDelta(float delta) {
    if (frames.empty()) return;
    elapsed_frame_time += delta;
    if (frames[current_frame_index].time <= elapsed_frame_time) {
        if (++current_frame_index >= (int)frames.size()) {
            current_frame_index = 0;
        }
        elapsed_frame_time = 0.0f;
    }
}

void Animation::Reset() {
    elapsed_frame_time = 0.0f;
    current_frame_index = 0;
}

void a2d::Animator::AddAnimation(const std::string &name, const pAnimation &animation) {
    animations[name] = animation;
}

void Animator::PlayAnimation(const std::string &name, float reset) {
    auto a = animations.find(name);
    if (a != animations.end()) {
        if (current_animation != a->second || reset) a->second->Reset();
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

Animator::Animator() : playing(false), current_animation(nullptr) {

}

void Animator::Update() {
    if (!current_animation) return;
    auto sprite = GetObject2D()->GetComponent<Sprite>();
    if (!sprite) return;
    auto frame = current_animation->GetCurrentFrame();
    if (!frame) return;
    sprite->SetTextureRegion(frame->texture_region);
    if (playing) current_animation->AddDelta(a2d::Engine::GetDeltaTime());
}

Animator::~Animator() {}

} //namespace a2d
