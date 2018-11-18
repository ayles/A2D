//
// Created by selya on 18.11.2018.
//

#ifndef DESKTOP_TRUMP_H
#define DESKTOP_TRUMP_H

#include <a2d/core/component.h>
#include <a2d/math.h>
#include <a2d/core/components/animator.h>
#include <a2d/core/components/sprite.h>

class Trump : public a2d::Component {
    a2d::Vector2f direction = a2d::Vector2f(1.0f, 0.0f);
    float speed = 3.0f;

    void Update() override {
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
        }
        GetObject2D()->position += direction * a2d::Engine::GetDeltaTime() * speed;
    }
};

#endif //DESKTOP_TRUMP_H
