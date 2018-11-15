//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <a2d/core.h>
#include <a2d/core/components/sprite.h>
#include <a2d/core/texture.h>

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
    float f = 0.0f;

    void Initialize() override {
        pCamera cam = new Camera();

        auto cam_obj = a2d::Engine::GetRoot()->AddChild(new Object2D);
        cam_obj->rotation = 1;
        a2d::Engine::SetCamera(cam_obj->AddComponent<Camera>());
        o1 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o1->AddComponent<Sprite>();
        o2 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o2->AddComponent<Sprite>();

        o1->GetComponent<Sprite>()->color.Set(0.5, 0.5, 1, 1);
        o2->GetComponent<Sprite>()->color.Set(1, 1, 0, 1);

        unsigned int width, height;
        std::vector<unsigned char> image;
        std::vector<unsigned char> raw_texture = a2d::FileSystem::LoadRaw("valeriy.png");
        lodepng::decode(image, width, height, raw_texture);

        (new a2d::Texture(width, height, &image[0], true))->Bind();
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
