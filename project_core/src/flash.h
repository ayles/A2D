//
// Created by selya on 09.01.2019.
//

#ifndef CORE_FLASH_H
#define CORE_FLASH_H

#include <a2d/a2d.hpp>

class Particle : public a2d::Component {
public:
    float time = 0.0;
    float max_time;
    float f1;
    float f2;
    float f3;
    a2d::Vector2f direction;
    a2d::Vector2f p1;
    a2d::Vector2f p2;

    void Initialize() override {
        auto s = GetObject2D()->AddComponent<a2d::Sprite>();
        s->SetShader(a2d::Resources::Get<a2d::Shader>("color"));
        s->SetSize(2, 2);
        s->SetOrigin(0.5f, 0.5f);
        float r = 0.9f + 0.1f * float(std::rand()) / RAND_MAX;
        float g = 0.9f + 0.1f * float(std::rand()) / RAND_MAX;
        float b = 0.4f + 0.5f * float(std::rand()) / RAND_MAX;
        f1 = float(std::rand()) / RAND_MAX;
        f2 = float(std::rand()) / RAND_MAX;
        f3 = float(std::rand()) / RAND_MAX;
        s->color = a2d::Vector4f(r, g, b, 1);
        max_time = float(std::rand()) / RAND_MAX * 3;
        p1 = GetObject2D()->position;
    }

    void Update() override {
        time += a2d::Engine::GetDeltaTime();
        p1 += direction;
        p2 = direction.Normalized().Perpendicular() * (3 * f1) * std::sin(100 * f3 + 3 * time * (0.1f + f2));
        GetObject2D()->position = p1 + p2;
        const auto &s = GetObject2D()->GetComponent<a2d::Sprite>();
        if (s) s->color.w = 1 - time * time / max_time / max_time;
        if (time >= max_time) GetObject2D()->Destroy();
    }
};

class Flash : public a2d::Component {
protected:
    void Update() override {
        auto particle = new a2d::Object2D;
        GetObject2D()->position = Engine::GetCamera()->ScreenToWorld(Input::GetMousePosition());
        particle->position = GetObject2D()->position;
        particle->AddComponent<Particle>()->direction = a2d::Vector2f(float(std::rand()) / RAND_MAX * 2 - 1,
                                                                      float(std::rand()) / RAND_MAX * 2 - 1).Normalized() / 3;
        particle->SetLayer(GetObject2D()->GetLayer());
    }
};

#endif //CORE_FLASH_H
