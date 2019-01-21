//
// Created by selya on 09.01.2019.
//

#include <a2d/graphics/animation.h>


namespace a2d {

Animation::Frame::Frame(const a2d::pTextureRegion &texture_region, float duration)
: texture_region(texture_region), duration(duration) {}


Animation::Animation(const std::vector<Frame> &frames) : Animation(std::vector<Frame>(frames)) {}

Animation::Animation(std::vector<Frame> &&frames) : frames(std::move(frames)), play_mode(PlayMode::NORMAL) {
    float time = 0.0f;
    for (auto &frame : frames) {
        frames_start_time.emplace(time, frame);
        time += frame.duration;
    }
}

const Animation::Frame *Animation::GetFrame(float animation_time) {
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

const Animation::Frame *Animation::GetFrameByIndex(int frame_index) {
    if (frame_index < 0 || frame_index >= frames.size()) return nullptr;
    return &frames[frame_index];
}

Animation::PlayMode Animation::GetPlayMode() const {
    return play_mode;
}

float Animation::GetDuration() const {
    if (frames.empty()) return 0.0f;
    return frames_start_time.rbegin()->first + frames[frames.size() - 1].duration;
}

void Animation::SetPlayMode(Animation::PlayMode play_mode) {
    this->play_mode = play_mode;
}

} //namespace a2d
