//
// Created by selya on 05.03.2019.
//

#include <a2d/core/animation/animation_curve.hpp>

namespace a2d {

AnimationCurve::AnimationCurve(const std::function<float(float)> &curve) : curve(curve) {}

const std::function<float (float)> AnimationCurve::Linear = [](float value) -> float {
    return value;
};

}

