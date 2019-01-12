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
        pObject2D camera_object = new Object2D;
        a2d::Engine::SetCamera(camera_object->AddComponent<PixelCamera>());
        camera_object->AddComponent<MoveCamera>();

        pObject2D text = new Object2D;
        text->AddComponent<Text>()->SetFont(new BitmapFont(FileSystem::LoadRaw("fonts/impact.ttf"), 48));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.5f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");

        pObject2D street_lamp = new Object2D;
        auto s = street_lamp->AddComponent<Sprite>();
        s->SetTextureRegion(new TextureRegion(Resources::Get<Texture>("street_lamp")));
        s->SetScaleFactor(2);
        street_lamp->SetLayer(3);

        pObject2D flash = new Object2D;
        flash->AddComponent<Flash>();
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
