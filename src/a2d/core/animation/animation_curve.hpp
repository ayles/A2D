//
// Created by selya on 05.03.2019.
//

#ifndef A2D_ANIMATION_CURVE_HPP
#define A2D_ANIMATION_CURVE_HPP

#include <a2d/core/macro.hpp>

namespace a2d {

class AnimationCurve {
    std::function<float (float)> curve;

public:
    AnimationCurve(const std::function<float (float)> &curve);

    float operator()(float f) const {
        return curve(f);
    }

    static const std::function<float (float)> Linear;
    static const std::function<float (float)> Circle;
    static const std::function<float (float)> CircleOut;
};

}

#endif //A2D_ANIMATION_CURVE_HPP
