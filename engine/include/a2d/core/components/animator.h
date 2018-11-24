#include <utility>

//
// Created by selya on 18.11.2018.
//

#ifndef A2D_ANIMATOR_H
#define A2D_ANIMATOR_H

#include <a2d/core/component.h>
#include <a2d/core/texture.h>
#include <a2d/core/engine.h>
#include <a2d/core/components/sprite.h>

#include <vector>

namespace a2d {

DECLARE_SMART_POINTER(Animation)

class Animation : public ref_counter {
public:
    class Frame {
    public:
        pTextureRegion texture_region;
        float time;

        Frame(const pTextureRegion &texture_region, float time) : texture_region(texture_region), time(time) {

        }
    };

    explicit Animation(std::vector<Frame> frames) : frames(std::move(frames)) {

    }

    const Frame *GetCurrentFrame() const {
        if (frames.empty()) return nullptr;
        return &frames[current_frame_index];
    }

    void AddDelta(float delta) {
        if (frames.empty()) return;
        elapsed_frame_time += delta;
        if (frames[current_frame_index].time <= elapsed_frame_time) {
            if (++current_frame_index >= frames.size()) {
                current_frame_index = 0;
            }
            elapsed_frame_time = 0.0f;
        }
    }

    void Reset() {
        elapsed_frame_time = 0.0f;
        current_frame_index = 0;
    }

private:
    float elapsed_frame_time = 0.0f;
    int current_frame_index = 0;
    const std::vector<Frame> frames;
};

class Animator : public Component {
    friend class Object2D;
public:
    void AddAnimation(const std::string &name, const pAnimation &animation) {
        animations[name] = animation;
    }

    void PlayAnimation(const std::string &name, float reset = true) {
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

    void PauseAnimation() {
        playing = false;
    }

private:
    bool playing = false;
    pAnimation current_animation = nullptr;
    std::map<std::string, pAnimation> animations;

    Animator() {}

    void Update() override {
        if (!current_animation) return;
        auto sprite = GetObject2D()->GetComponent<Sprite>();
        if (!sprite) sprite = GetObject2D()->GetComponent<PixelSprite>();
        if (!sprite) return;
        auto frame = current_animation->GetCurrentFrame();
        if (!frame) return;
        sprite->SetTextureRegion(frame->texture_region);
        if (playing) current_animation->AddDelta(a2d::Engine::GetDeltaTime());
    }

    ~Animator() override {}
};

}//namespace a2d

#endif //A2D_ANIMATOR_H
