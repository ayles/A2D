//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <a2d/a2d.hpp>

#include <cmath>
#include <string>

namespace a2d {

class RootComponent : public Component {
public:
    pObject2D text;

    void Initialize() override {
        auto camera = Object2D::Create();
        a2d::Engine::SetCamera(camera->AddComponent<PixelCamera>());

        text = Object2D::Create();
        text->AddComponent<Text>()->SetFont(BitmapFont::Create(FileSystem::LoadRaw("fonts/impact.ttf"), 48));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.0f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");
    }

    void Update() override {
        text->rotation += Engine::GetDeltaTime();
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
