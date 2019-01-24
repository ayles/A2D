//
// Created by selya on 09.01.2019.
//

#ifndef A2D_ANIMATION_H
#define A2D_ANIMATION_H

#include <a2d/core/ref_counter.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/graphics/texture_region.hpp>

#include <vector>
#include <map>

namespace a2d {

DECLARE_SMART_POINTER(Animation)

class Animation : public ref_counter {
public:
    class Frame {
    public:
        pTextureRegion texture_region;
        float duration;

        Frame(const pTextureRegion &texture_region, float duration);
    };

    enum PlayMode {
        NORMAL,
        LOOP,
        LOOP_MIRRORED
    };

private:
    std::vector<Frame> frames;
    std::map<float, Frame> frames_start_time;
    PlayMode play_mode;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Animation)

    const Frame *GetFrameByTime(float animation_time);
    const Frame *GetFrame(int frame_index);

    PlayMode GetPlayMode() const;
    float GetDuration() const;

    void SetPlayMode(PlayMode play_mode);

    static pAnimation Create(const std::vector<Frame> &frames);
    static pAnimation Create(std::vector<Frame> &&frames);

private:
    Animation(const std::vector<Frame> &frames);
    Animation(std::vector<Frame> &&frames);
};

} //namespace a2d

#endif //A2D_ANIMATION_H
