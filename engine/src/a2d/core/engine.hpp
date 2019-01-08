//
// Created by selya on 26.10.2018.
//

#ifndef A2D_ENGINE_H
#define A2D_ENGINE_H

#include <a2d/core/macro.hpp>

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
    static void SetCamera(intrusive_ptr<Camera> camera);

    static unsigned long long GetFrameIndex();
    static float GetDeltaTime();
    static intrusive_ptr<Object2D> &GetRoot();
    static intrusive_ptr<Camera> &GetCamera();
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

    static unsigned long long frame_index;
    static float delta_time;
    static intrusive_ptr<Object2D> root;
    static intrusive_ptr<Camera> camera;
    static std::shared_ptr<spdlog::logger> logger;
    static std::thread::id ui_thread_id;
    static bool playing;
};

} //namespace a2d

#endif //A2D_ENGINE_H
