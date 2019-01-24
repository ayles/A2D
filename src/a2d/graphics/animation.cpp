//
// Created by selya on 09.01.2019.
//

#include <a2d/graphics/animation.h>
#include <a2d/core/engine.hpp>
#include "animation.h"


namespace a2d {

Animation::Frame::Frame(const a2d::pTextureRegion &texture_region, float duration)
: texture_region(texture_region), duration(duration) {
    ASSERT_MAIN_THREAD
}


Animation::Animation(const std::vector<Frame> &frames) : Animation(std::vector<Frame>(frames)) {
    ASSERT_MAIN_THREAD
}

Animation::Animation(std::vector<Frame> &&frames) : frames(std::move(frames)), play_mode(PlayMode::NORMAL) {
    ASSERT_MAIN_THREAD
    float time = 0.0f;
    for (auto &frame : frames) {
        frames_start_time.emplace(time, frame);
        time += frame.duration;
    }
}

const Animation::Frame *Animation::GetFrameByTime(float animation_time) {
    ASSERT_MAIN_THREAD
    switch (play_mode) {
        case PlayMode::LOOP: {
            animation_time = std::fmod(animation_time, GetDuration());
            break;
        }

        case PlayMode::LOOP_MIRRORED: {
            bool mirror = bool(int(animation_time / GetDuration()) % 2);
            animation_time = std::fmod(animation_time, GetDuration());
            if (mirror) animation_time = GetDuration() - animation_time;
            break;
        }

        case PlayMode::NORMAL: break;
    }

    return &(--frames_start_time.lower_bound(animation_time))->second;
}

const Animation::Frame *Animation::GetFrame(int frame_index) {
    ASSERT_MAIN_THREAD
    if (frame_index < 0 || frame_index >= frames.size()) return nullptr;
    return &frames[frame_index];
}

Animation::PlayMode Animation::GetPlayMode() const {
    ASSERT_MAIN_THREAD
    return play_mode;
}

float Animation::GetDuration() const {
    ASSERT_MAIN_THREAD
    if (frames.empty()) return 0.0f;
    return frames_start_time.rbegin()->first + frames[frames.size() - 1].duration;
}

void Animation::SetPlayMode(Animation::PlayMode play_mode) {
    ASSERT_MAIN_THREAD
    this->play_mode = play_mode;
}

pAnimation Animation::Create(const std::vector<Animation::Frame> &frames) {
    return new Animation(frames);
}

pAnimation Animation::Create(std::vector<Animation::Frame> &&frames) {
    return new Animation(std::move(frames));
}

} //namespace a2d
