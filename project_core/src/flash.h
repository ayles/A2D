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
    a2d::Vector2f direction;

    void Initialize() override {
        auto s = GetObject2D()->AddComponent<a2d::Sprite>();
        s->shader = a2d::Resources::Get<a2d::Shader>("color");
        s->SetSize(2, 2);
        s->SetOrigin(0.5f, 0.5f);
        s->color = a2d::Vector4f(1, 1, 0.7f, 1);
        max_time = float(std::rand()) / RAND_MAX * 2;
    }

    void Update() override {
        time += a2d::Engine::GetDeltaTime();
        if (time >= max_time) GetObject2D()->Destroy();
        GetObject2D()->position += direction;
        const auto &s = GetObject2D()->GetComponent<a2d::Sprite>();
        if (s) s->color.w = 1 - time * time / max_time / max_time;
    }
};

class Flash : public a2d::Component {
protected:
    void Update() override {
        for (int i = 0; i < 10; ++i) {
            GetObject2D()->position = Engine::GetCamera()->ScreenToWorld(Input::GetMousePosition());
            auto particle = new a2d::Object2D;
            particle->position = GetObject2D()->position;
            particle->AddComponent<Particle>()->direction = a2d::Vector2f(float(std::rand()) / RAND_MAX * 2 - 1,
                                                                          float(std::rand()) / RAND_MAX * 2 - 1).Normalized();
        }
    }
};

#endif //CORE_FLASH_H
