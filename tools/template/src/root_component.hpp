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
        pObject2D camera = new Object2D;
        a2d::Engine::SetCamera(camera->AddComponent<PixelCamera>());

        text = new Object2D;
        text->AddComponent<Text>()->SetFont(new BitmapFont(FileSystem::LoadRaw("fonts/impact.ttf"), 48));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.5f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");
    }

    void Update() override {
        text->rotation += Engine::GetDeltaTime();
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
