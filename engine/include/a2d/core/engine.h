//
// Created by selya on 26.10.2018.
//

#ifndef A2D_ENGINE_H
#define A2D_ENGINE_H

#include <a2d/core/macro.h>
#include <a2d/core/texture.h>
#include <a2d/core/object2d.h>
#include <a2d/math.h>
#include <a2d/core/components/camera.h>

#include <spdlog/spdlog.h>

#include <thread>


namespace a2d {

class Engine {
    friend class Renderer;
    friend class NativeRenderer;
    friend class NativeConnector;

#ifdef TARGET_DESKTOP
    friend int main();
#endif

public:
    static void SetCamera(pCamera camera);

    static float GetDeltaTime();
    static pObject2D GetRoot();
    static pCamera GetCamera();
    static std::shared_ptr<spdlog::logger> GetLogger();
    static std::thread::id GetUIThreadID();

    Engine() = delete;

private:
    static bool Initialize();
    static bool Update();
    static bool PostUpdate();
    static bool PreDraw();
    static bool PostDraw();
    static void OnPause();
    static void OnResume();
    static void Uninitialize();

    static float delta_time;
    static pObject2D root;
    static pCamera camera;
    static std::shared_ptr<spdlog::logger> logger;
    static std::thread::id ui_thread_id;
};

} //namespace a2d

#endif //A2D_ENGINE_H
