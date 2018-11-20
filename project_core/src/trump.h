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

class Trump : public a2d::Component {
    a2d::Vector2f direction = a2d::Vector2f(0.0f, 0.0f);
    float speed = 3.0f;

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

        /*
        if (direction.x > 0) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("right", false);
            if (GetObject2D()->position.x > a2d::Engine::GetCamera()->GetWidth() / 2 -
                GetObject2D()->scale.x / 2) direction.Set(0.0f, 1.0f);
        } else if (direction.x < 0) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("left", false);
            if (GetObject2D()->position.x < -a2d::Engine::GetCamera()->GetWidth() / 2 +
                GetObject2D()->scale.x / 2) direction.Set(0.0f, -1.0f);
        } else if (direction.y > 0) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("top", false);
            if (GetObject2D()->position.y > a2d::Engine::GetCamera()->GetHeight() / 2 -
                GetObject2D()->scale.y / 2) direction.Set(-1.0f, 0.0f);
        } else if (direction.y < 0) {
            GetObject2D()->GetComponent<a2d::Animator>()->PlayAnimation("bottom", false);
            if (GetObject2D()->position.y < -a2d::Engine::GetCamera()->GetHeight() / 2 +
                GetObject2D()->scale.y / 2) direction.Set(1.0f, 0.0f);
        }*/
        GetObject2D()->position += direction * a2d::Engine::GetDeltaTime() * speed;
    }
};

#endif //DESKTOP_TRUMP_H
