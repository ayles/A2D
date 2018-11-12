//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <A2D/core.h>
#include <A2D/core/components/sprite.h>

#include <cmath>

namespace a2d {

class RootComponent : public Component {
public:
    a2d::pObject2D o1;
    a2d::pObject2D o2;
    float f = 0.0f;

    void Initialize() override {
        auto cam_obj = a2d::Engine::GetRoot()->AddChild(new Object2D);
        a2d::Engine::SetCamera(cam_obj->AddComponent<Camera>());
        o1 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o1->AddComponent<Sprite>();
        o2 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o2->AddComponent<Sprite>();

        o1->GetComponent<Sprite>()->color.Set(1, 0, 0, 1);
        o2->GetComponent<Sprite>()->color.Set(0, 1, 0, 1);
    }

    void Update() override {
        f += a2d::Engine::GetDeltaTime();
        if (std::sin(f * 100) < 0) {
            o1->SetLayer(0);
            o2->SetLayer(1);
        } else {
            o1->SetLayer(1);
            o2->SetLayer(0);
        }

        o1->position.Set(std::sin(f) * 3, 0);
        o2->position.Set(-std::sin(f) * 3, 0);
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
