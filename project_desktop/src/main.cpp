//
// Created by selya on 05.11.2018.
//

#include <a2d/a2d.h>
#include <a2d/graphics/renderer.h>
#include <root_component.h>
#include <a2d/filesystem/filesystem.h>
#include <a2d/input/input.h>
#include <a2d/core/audio.h>


#if defined(TARGET_WINDOWS) && defined(FORCE_DISCRETE_GPU)
// Enable NVIDIA or AMD discrete gpu
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

namespace a2d {

class NativeConnector {
public:
    static bool Initialize() {
        if (!a2d::Audio::Initialize()) return false;
        if (!a2d::Engine::Initialize()) return false;
        if (!a2d::Renderer::Initialize()) return false;
        if (!a2d::Input::Initialize()) return false;
        if (!a2d::Physics::Initialize()) return false;
        a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
        return true;
    }

    static bool Step() {
        return a2d::Physics::Step() &&
               a2d::Engine::Update() &&
               a2d::Engine::PostUpdate() &&
               a2d::Engine::PreDraw() &&
               a2d::Renderer::Draw() &&
               a2d::Engine::PostDraw();
    }

    static void Uninitialize() {
        a2d::Physics::Uninitialize();
        a2d::Input::Uninitialize();
        a2d::Engine::Uninitialize();
        a2d::Renderer::Uninitialize();
        a2d::Audio::Uninitialize();
    }
};

}

int main() {
#ifdef TARGET_LINUX
    setenv("DISPLAY", ":0", true);
    setenv("LIBGL_ALWAYS_INDIRECT", "1", true);
#endif

    if (!a2d::NativeConnector::Initialize()) return -1;
    while (a2d::NativeConnector::Step());
    a2d::NativeConnector::Uninitialize();

    return 0;
}