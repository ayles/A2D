//
// Created by selya on 18.11.2018.
//

#ifndef DESKTOP_TRUMP_H
#define DESKTOP_TRUMP_H

#include <a2d/core/component.h>
#include <a2d/math.h>
#include <a2d/core/components/animator.h>
#include <a2d/core/components/sprite.h>
#include <a2d/core/input.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/audio_source.h>

class Trump : public a2d::Component {
    a2d::Vector2f direction = a2d::Vector2f(0.0f, 0.0f);
    float speed = 200.0f;

    void Update() override {
        direction = 0;

#ifdef TARGET_DESKTOP

        if (a2d::Input::GetKey(a2d::Input::KeyCode::KEY_W) == a2d::Input::KeyState::PRESS) direction.y += 1;
        if (a2d::Input::GetKey(a2d::Input::KeyCode::KEY_S) == a2d::Input::KeyState::PRESS) direction.y -= 1;
        if (a2d::Input::GetKey(a2d::Input::KeyCode::KEY_A) == a2d::Input::KeyState::PRESS) direction.x -= 1;
        if (a2d::Input::GetKey(a2d::Input::KeyCode::KEY_D) == a2d::Input::KeyState::PRESS) direction.x += 1;

#elif TARGET_ANDROID
        auto touch = a2d::Input::GetTouch(0);
        auto pos = a2d::Engine::GetCamera()->ScreenToWorld(
                a2d::Vector2f(touch.x / a2d::Renderer::GetWidth(), 1 - touch.y / a2d::Renderer::GetHeight())
        );
        direction = pos - GetObject2D()->position;

        auto diff = pos - GetObject2D()->position;
        if (diff.LengthSquared() < 0.01f) {
            GetObject2D()->GetComponent<a2d::Animator>()->PauseAnimation();
            return;
        }
#endif
        if (direction.LengthSquared() > 0)
            direction.Normalize();

        if (direction.x > 0 && std::abs(direction.x) >= std::abs(direction.y)) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("right", false);
        } else if (direction.x < 0 && std::abs(direction.x) >= std::abs(direction.y)) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("left", false);
        } else if (direction.y > 0) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("top", false);
        } else if (direction.y < 0) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("bottom", false);
        } else {
            GetObject2D()->GetComponent<a2d::Animator>()->PauseAnimation();
        }

        GetObject2D()->position += direction * a2d::Engine::GetDeltaTime() * speed;

        GetObject2D()->GetComponent<a2d::AudioSource>()->Play();
        float pan = (GetObject2D()->position.x - a2d::Engine::GetCamera()->GetObject2D()->position.x) / (a2d::Engine::GetCamera()->GetWidth() / 2);
        if (pan > 1) pan = 1;
        if (pan < -1) pan = -1;
        GetObject2D()->GetComponent<a2d::AudioSource>()->SetPan(pan);
    }
};

#endif //DESKTOP_TRUMP_H
