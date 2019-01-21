#include <utility>

//
// Created by selya on 18.11.2018.
//

#ifndef A2D_ANIMATOR_H
#define A2D_ANIMATOR_H

#include <a2d/core/component.hpp>
#include <a2d/graphics/texture_region.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/components/sprite.hpp>
#include <a2d/graphics/animation.h>

#include <vector>

namespace a2d {

class Animator : public Component {
    friend class Object2D;
public:
    void AddAnimation(const std::string &name, const pAnimation &animation);
    void PlayAnimation(const std::string &name, bool reset = true);
    void PauseAnimation();

private:
    float time;
    bool playing;
    pAnimation current_animation;
    std::map<std::string, pAnimation> animations;

    Animator();
    ~Animator() override;

    void Update() override;
};

}//namespace a2d

#endif //A2D_ANIMATOR_H
