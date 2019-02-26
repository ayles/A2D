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
    pCamera camera;

    void Initialize() override {
        camera = Object2D::Create()->AddComponent<Camera>();
        Renderer::SetMainCamera(camera);

        text = Object2D::Create();
        text->AddComponent<Text>()->SetFont(Resources::Get<BitmapFont>("impact"));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.0f);
        text->GetComponent<Text>()->SetFontSize(24);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");
    }

    void Update() override {
        text->SetRotation(text->GetRotation() + Engine::GetDeltaTime());
        camera->SetHeight(Renderer::GetHeight());
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
