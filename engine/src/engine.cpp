//
// Created by selya on 26.10.2018.
//

#include <A2D/renderer.h>
#include <A2D/core/engine.h>

#include <chrono>
#include <cmath>
#include <root_component.h>

a2d::pObject2D a2d::Engine::root = new Object2D;
a2d::pCamera a2d::Engine::camera = nullptr;
float a2d::Engine::delta_time = 0.0f;

bool a2d::Engine::Initialize() {
    // Important for hierarchical activate/disable
    root->is_active = true;
    root->AddComponent<RootComponent>();

    return true;
}

bool a2d::Engine::Update() {
    static auto start = std::chrono::system_clock::now();

    delta_time = std::chrono::duration<float>(std::chrono::system_clock::now() - start).count();
    start = std::chrono::system_clock::now();

    root->Update();

    return true;
}

bool a2d::Engine::PostUpdate() {
    root->PostUpdate();
    return true;
}

bool a2d::Engine::PreDraw() {
    root->PreDraw(Matrix4f());
    return true;
}

bool a2d::Engine::PostDraw() {
    root->PostDraw();
    return true;
}

void a2d::Engine::OnPause() {

}

void a2d::Engine::OnResume() {

}

void a2d::Engine::Uninitialize() {

}

float a2d::Engine::GetDeltaTime() {
    return delta_time;
}

a2d::pObject2D a2d::Engine::GetRoot() {
    return root;
}

a2d::pCamera a2d::Engine::GetCamera() {
    return camera;
}

void a2d::Engine::SetCamera(a2d::pCamera camera) {
    Engine::camera = camera;
}
