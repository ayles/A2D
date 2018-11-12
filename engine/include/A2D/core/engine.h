//
// Created by selya on 26.10.2018.
//

#ifndef A2D_ENGINE_H
#define A2D_ENGINE_H

#include <A2D/core/macro.h>
#include <A2D/core/object2d.h>
#include <A2D/math.h>
#include <A2D/core/components/camera.h>

#include <spdlog/spdlog.h>

namespace a2d {

class Engine {
    friend class Renderer;
    friend class NativeRenderer;

public:
    static pObject2D GetRoot();
    static pCamera GetCamera();
    static void SetCamera(pCamera camera);
    static float GetDeltaTime();
    static std::shared_ptr<spdlog::logger> GetLogger();

    Engine() = delete;

    static bool Initialize();
    static bool Update();
    static bool PostUpdate();
    static bool PreDraw();
    static bool PostDraw();
    static void OnPause();
    static void OnResume();
    static void Uninitialize();

private:
    static float delta_time;
    static pObject2D root;
    static pCamera camera;
    static std::shared_ptr<spdlog::logger> logger;
};

} //namespace a2d

#endif //A2D_ENGINE_H
