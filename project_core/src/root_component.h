//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <a2d/core.h>

#include "move_character.h"
#include "move_camera.h"

#include <cmath>
#include <string>

namespace a2d {

class RootComponent : public Component {
public:
    a2d::pObject2D o1;
    a2d::pObject2D bunny;

    void Initialize() override {
        auto camera_object = a2d::Engine::GetRoot()->AddChild(new Object2D);
        a2d::Engine::SetCamera(camera_object->AddComponent<PixelCamera>());
        camera_object->AddComponent<MoveCamera>();

        auto text = a2d::Engine::GetRoot()->AddChild(new Object2D);
        text->AddComponent<Text>()->SetFont(new BitmapFont(FileSystem::LoadRaw("fonts/impact.ttf"), 96));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.5f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");

        auto bunny_texture = Texture::GetTexture("bunny");

        std::vector<Animation::Frame> f_right;
        std::vector<Animation::Frame> f_left;

        int frames = 6;
        float frame_time = 0.08f;
        for (int j = 0; j < frames; ++j) {
            f_right.emplace_back(
                    new TextureRegion(
                            bunny_texture,
                            j * bunny_texture->width / frames, 0,
                            bunny_texture->width / frames, bunny_texture->height,
                            Texture::Filtering::NEAREST),
                    frame_time
            );
            f_left.emplace_back(
                    new TextureRegion(
                            bunny_texture,
                            (j + 1) * bunny_texture->width / frames, 0,
                            -bunny_texture->width / frames, bunny_texture->height,
                            Texture::Filtering::NEAREST),
                    frame_time
            );
        }

        bunny = a2d::Engine::GetRoot()->AddChild(new Object2D);
        bunny->AddComponent<Sprite>()->SetOrigin(0.5f, 0.5f);
        bunny->scale.Set(2);
        auto a = bunny->AddComponent<Animator>();
        a->AddAnimation("right", new Animation(f_right));
        a->AddAnimation("left", new Animation(f_left));
        bunny->SetLayer(1);
        bunny->AddComponent<MoveCharacter>();
        bunny->AddComponent<AudioSource>()->SetAudioClip(new AudioClip(a2d::FileSystem::LoadRaw("audio/just.mp3")));
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
