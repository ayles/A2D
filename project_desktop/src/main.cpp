//
// Created by selya on 05.11.2018.
//

#include <A2D/core.h>
#include <A2D/renderer.h>
#include <root_component.h>

#include "native_renderer.h"

int main() {
    if (!a2d::Engine::Initialize()) return -1;
    a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
    if (!a2d::NativeRenderer::Initialize()) return -1;

    while (a2d::Engine::Update() &&
            a2d::Engine::PostUpdate() &&
            a2d::Engine::PreDraw() &&
            a2d::NativeRenderer::Draw() &&
            a2d::Engine::PostDraw());

    a2d::Engine::Uninitialize();
    a2d::NativeRenderer::Uninitialize();

    return 0;
}