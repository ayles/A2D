//
// Created by selya on 26.10.2018.
//

#ifndef A2D_ENGINE_H
#define A2D_ENGINE_H

#include <a2d/core/macro.h>

#include <spdlog/spdlog.h>

#include <thread>

namespace a2d {

class Object2D;
class Camera;

class Engine {
    friend class Renderer;
    friend class NativeRenderer;
    friend class NativeConnector;

public:
    static void SetCamera(SMART_POINTER(Camera) camera);

    static float GetDeltaTime();
    static SMART_POINTER(Object2D) &GetRoot();
    static SMART_POINTER(Camera) &GetCamera();
    static std::shared_ptr<spdlog::logger> &GetLogger();
    static std::thread::id &GetUIThreadID();
    static bool IsPlaying();

    Engine() = delete;
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Engine)
    ~Engine() = delete;

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
    static SMART_POINTER(Object2D) root;
    static SMART_POINTER(Camera) camera;
    static std::shared_ptr<spdlog::logger> logger;
    static std::thread::id ui_thread_id;
    static bool playing;
};

} //namespace a2d

#endif //A2D_ENGINE_H
