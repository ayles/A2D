//
// Created by selya on 05.11.2018.
//

#include <a2d/core.h>
#include <a2d/core/renderer.h>
#include <root_component.h>

#include <a2d/core/native_renderer.h>

// Enable NVIDIA or AMD discrete gpu
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace a2d {

class NativeConnector {
public:
    static bool Initialize() {
        if (!a2d::Engine::Initialize()) return false;
        if (!a2d::NativeRenderer::Initialize()) return false;
        a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
        return true;
    }

    static bool Step() {
        return a2d::Engine::Update() &&
               a2d::Engine::PostUpdate() &&
               a2d::Engine::PreDraw() &&
               a2d::NativeRenderer::Draw() &&
               a2d::Engine::PostDraw();
    }

    static void Uninitialize() {
        a2d::Engine::Uninitialize();
        a2d::NativeRenderer::Uninitialize();
    }
};

}

int main() {
    if (!a2d::NativeConnector::Initialize()) return -1;
    while (a2d::NativeConnector::Step());
    a2d::NativeConnector::Uninitialize();

    return 0;
}