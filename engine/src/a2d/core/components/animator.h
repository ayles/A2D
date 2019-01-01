#include <utility>

//
// Created by selya on 18.11.2018.
//

#ifndef A2D_ANIMATOR_H
#define A2D_ANIMATOR_H

#include <a2d/core/component.h>
#include <a2d/graphics/texture_region.h>
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

        Frame(const pTextureRegion &texture_region, float time);
    };

    Animation(std::vector<Frame> frames);

    const Frame *GetCurrentFrame() const;

    void AddDelta(float delta);
    void Reset();

private:
    float elapsed_frame_time;
    int current_frame_index;
    const std::vector<Frame> frames;
};

class Animator : public Component {
    friend class Object2D;
public:
    void AddAnimation(const std::string &name, const pAnimation &animation);
    void PlayAnimation(const std::string &name, float reset = true);
    void PauseAnimation();

private:
    bool playing;
    pAnimation current_animation;
    std::map<std::string, pAnimation> animations;

    Animator();

    void Update() override;

    ~Animator() override;
};

}//namespace a2d

#endif //A2D_ANIMATOR_H
