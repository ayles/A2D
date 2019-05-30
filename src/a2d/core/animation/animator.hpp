//
// Created by selya on 14.03.2019.
//

#ifndef A2D_ANIMATOR_HPP
#define A2D_ANIMATOR_HPP

#include <a2d/core/component.hpp>
#include <a2d/core/animation/animation_curve.hpp>

namespace a2d {

template<class Animation>
class Animator : public Component {
    Animation animation;
    float duration;
    int times;
    bool mirror;
    float delay;
    AnimationCurve curve;

    float elapsed;
    float elapsed_delay;
    bool paused;
    bool started;

public:
    Animator(const Animation &animation, float duration, int times = 1, bool mirror = false, float delay = 0.0f,
            bool start_paused = false, AnimationCurve curve = AnimationCurve::Linear) :
            animation(animation), duration(duration), times(times), mirror(mirror), delay(delay), paused(start_paused),
            started(false), curve(std::move(curve)),
            elapsed(0), elapsed_delay(0) {};

    void SetPaused(bool paused) {
        this->paused = paused;
    }

    void SetDuration(float duration) {
        this->elapsed = duration;
        Update();
    }

    void SetNormalizedDuration(float normalized_duration) {
        this->elapsed = duration * normalized_duration;
        Update();
    }

    Animation &GetAnimation() {
        return animation;
    }

private:
    void Update() override {
        if (delay > 0 && elapsed_delay < delay) {
            if (!paused) elapsed_delay += Engine::GetDeltaTime();
            return;
        }

        if (!started) {
            this->animation.object_2d = GetObject2D().get();
            animation.Start();
            started = true;
        }

        float t = std::fmod(elapsed, duration) / duration;
        if (mirror && int(elapsed / duration) & 1) t = 1 - t;
        animation.Step(curve(t));

        if (!paused) elapsed += Engine::GetDeltaTime();
        if (times > 0 && elapsed > duration * times) {
            t = 1;
            if (mirror && int(elapsed / duration) & 1) t = 1 - t;
            animation.Step(curve(t));
            Destroy();
        }
    };

    void OnDestroy() override {
        animation.End();
    }
};

}

#endif //A2D_ANIMATOR_HPP
