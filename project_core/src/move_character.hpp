//
// Created by selya on 18.11.2018.
//

#ifndef DESKTOP_TRUMP_H
#define DESKTOP_TRUMP_H

#include <a2d/core/component.hpp>
#include <a2d/math/math.hpp>
#include <a2d/core/components/animator.hpp>
#include <a2d/core/components/sprite.hpp>
#include <a2d/input/input.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/components/audio_source.hpp>
#include <a2d/core/components/physics/physics_body.hpp>

using namespace a2d;

class Particle : public Component {

    void Initialize() override {

    }
};

class Emitter : public Component {
    void Update() override {

    }
};

class MoveCharacter : public a2d::Component {
    a2d::Vector2f direction = a2d::Vector2f(0.0f, 0.0f);
    float speed = 200.0f;

    void Update() override {
        direction = 0;

        auto m = a2d::Input::GetMousePosition();
        auto pos = a2d::Engine::GetCamera()->ScreenToWorld(Vector2f(m.x, a2d::Renderer::GetHeight() - m.y));

        direction = pos - GetObject2D()->position;

        auto diff = pos - GetObject2D()->position;
        if (diff.LengthSquared() < 10.f) {
            GetObject2D()->GetComponent<a2d::Animator>()->PauseAnimation();
            return;
        }

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

        auto po = GetObject2D()->GetComponent<a2d::PhysicsBody>()->GetPosition();
        GetObject2D()->GetComponent<a2d::PhysicsBody>()->SetPosition(po + direction * a2d::Engine::GetDeltaTime() * speed);

        GetObject2D()->GetComponent<a2d::AudioSource>()->Play();
        float pan = (GetObject2D()->position.x - a2d::Engine::GetCamera()->GetObject2D()->position.x) / (a2d::Engine::GetCamera()->GetWidth() / 2);
        if (pan > 1) pan = 1;
        if (pan < -1) pan = -1;
        GetObject2D()->GetComponent<a2d::AudioSource>()->SetPan(pan);
    }
};

#endif //DESKTOP_TRUMP_H
