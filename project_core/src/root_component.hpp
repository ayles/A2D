//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <a2d/a2d.hpp>

#include "move_character.hpp"
#include "move_camera.hpp"
#include "flash.h"

#include <cmath>
#include <string>

namespace a2d {

class RootComponent : public Component {
public:

    void Initialize() override {
        auto camera_object = a2d::Engine::GetRoot()->AddChild(new Object2D);
        a2d::Engine::SetCamera(camera_object->AddComponent<PixelCamera>());
        camera_object->AddComponent<MoveCamera>();

        auto text = a2d::Engine::GetRoot()->AddChild(new Object2D);
        text->AddComponent<Text>()->SetFont(new BitmapFont(FileSystem::LoadRaw("fonts/impact.ttf"), 48));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.5f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");

        auto p = Engine::GetRoot()->AddChild(new Object2D);
        p->AddComponent<Flash>();
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
