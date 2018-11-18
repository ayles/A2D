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

class Animator : public Component {
    friend class Object2D;
public:
    void SetFrames(const std::vector<pTextureRegion> &frames, float frame_time) {
        this->frames = frames;
        this->frame_time = frame_time;
    }

private:
    std::vector<pTextureRegion> frames;
    float frame_time;
    float time = 0.0f;
    int current_frame = 0;

    Animator() {}

    void Update() override {
        if (frames.empty()) return;

        if (time > frame_time) {
            if (++current_frame >= (int)frames.size()) {
                current_frame = 0;
            }
            time = 0.0f;
        }

        GetObject2D()->GetComponent<Sprite>()->texture_region = frames[current_frame];
        time += a2d::Engine::GetDeltaTime();
    }

    ~Animator() override {}
};

}//namespace a2d

#endif //A2D_ANIMATOR_H
