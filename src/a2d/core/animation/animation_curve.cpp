//
// Created by selya on 05.03.2019.
//

#include <a2d/core/animation/animation_curve.hpp>
#include <cmath>

namespace a2d {

AnimationCurve::AnimationCurve(const std::function<float(float)> &curve) : curve(curve) {}

const std::function<float (float)> AnimationCurve::Linear = [](float value) -> float {
    return value;
};

const std::function<float (float)> AnimationCurve::Circle = [](float value) -> float {
    if (value <= 0.5f) {
        value *= 2;
        return (1 - (float)std::sqrt(1 - value * value)) / 2;
    }
    value--;
    value *= 2;
    return ((float)std::sqrt(1 - value * value) + 1) / 2;
};

const std::function<float (float)> AnimationCurve::CircleOut = [](float value) -> float {
    value--;
    return (float)std::sqrt(1 - value * value);
};

}

