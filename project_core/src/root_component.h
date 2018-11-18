//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <a2d/core.h>
#include <a2d/core/components/sprite.h>
#include <a2d/core/texture.h>
#include <a2d/core/components/animator.h>

#include <lodepng.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#endif

#include <cmath>

namespace a2d {

class RootComponent : public Component {
public:
    a2d::pObject2D o1;
    a2d::pObject2D o2;
    a2d::pObject2D e;
    float f = 0.0f;

    void Initialize() override {
        pCamera cam = new Camera();

        auto cam_obj = a2d::Engine::GetRoot()->AddChild(new Object2D);
        a2d::Engine::SetCamera(cam_obj->AddComponent<Camera>());

        auto valeriy = Texture::GetTexture("valeriy");

        o1 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o1->AddComponent<Sprite>()->texture_region = new TextureRegion(valeriy, 0, 0, 500, 200);
        o2 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o2->AddComponent<Sprite>()->texture_region = new TextureRegion(valeriy);

        o1->GetComponent<Sprite>()->color.Set(0.5, 0.5, 1, 1);
        o2->GetComponent<Sprite>()->color.Set(1, 1, 0, 1);

        auto explosion = Texture::GetTexture("trump");

        e = a2d::Engine::GetRoot()->AddChild(new Object2D);
        e->AddComponent<Sprite>()->texture_region = new TextureRegion(explosion);

        std::vector<pTextureRegion> frames;
        for (int i = 3; i >= 0; --i) {
            for (int j = 0; j < 6; ++j) {
                frames.push_back(new TextureRegion(explosion, j * explosion->width / 6, i * explosion->height / 4, explosion->width / 6, explosion->height / 4));
            }
        }

        e->AddComponent<Animator>()->SetFrames(frames, 0.08f);
        e->scale.Set(1.5f);
        e->SetLayer(4);
    }

    void Update() override {
        f += a2d::Engine::GetDeltaTime();
        /*if (std::sin(f * 100) < 0) {
            o1->SetLayer(0);
            o2->SetLayer(1);
        } else {
            o1->SetLayer(1);
            o2->SetLayer(0);
        }*/

        o1->GetComponent<Sprite>()->color.w = std::sin(f * 0.5f);
        o2->GetComponent<Sprite>()->color.w = std::sin(f * 0.25f);
        o1->position.Set(std::sin(f) * 3, 0.5f);
        o2->position.Set(-std::sin(f) * 3, -0.5f);
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
