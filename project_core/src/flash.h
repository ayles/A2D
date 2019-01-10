//
// Created by selya on 09.01.2019.
//

#ifndef CORE_FLASH_H
#define CORE_FLASH_H

#include <a2d/a2d.hpp>

class Particle : public a2d::Component {
public:
    float time = 0.0;
    a2d::Vector2f direction;

    void Initialize() override {
        auto s = GetObject2D()->AddComponent<a2d::Sprite>();
        s->shader = a2d::Resources::Get<a2d::Shader>("color");
        s->SetSize(10, 10);
        s->SetOrigin(0.5f, 0.5f);
        s->color = a2d::Vector4f(1, 1, 0.7f, 1);
    }

    void Update() override {
        time += a2d::Engine::GetDeltaTime();
        if (time >= 1.0f) GetObject2D()->Destroy();
        GetObject2D()->position += direction;
    }
};

class Flash : public a2d::Component {
protected:
    void Update() override {
        auto particle = GetObject2D()->AddChild(new a2d::Object2D);
        particle->AddComponent<Particle>()->direction = a2d::Vector2f(float(std::rand()) / RAND_MAX * 2 - 1, float(std::rand()) / RAND_MAX * 2 - 1);
    }
};

#endif //CORE_FLASH_H
