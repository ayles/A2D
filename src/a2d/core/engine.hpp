//
// Created by selya on 26.10.2018.
//

#ifndef A2D_ENGINE_H
#define A2D_ENGINE_H

#include <a2d/core/macro.hpp>
#include <a2d/core/command.hpp>
#include <a2d/core/commands/lambda_command.h>
#include <a2d/core/component.hpp>

#include <spdlog/spdlog.h>

#include <thread>
#include <queue>
#include <set>

namespace a2d {

class Object2D;
class Camera;

class Engine {
    friend class Renderer;
    friend class NativeRenderer;
    friend class NativeConnector;
    friend class Component;
    friend class Object2D;
    friend class Physics;

    static unsigned long long frame_index;
    static float delta_time;
    static intrusive_ptr<Object2D> root;
    static intrusive_ptr<Camera> camera;
    static std::shared_ptr<spdlog::logger> logger;
    static std::thread::id main_thread_id;
    static bool playing;
    static std::queue<pCommand> commands;
    static std::set<pComponent> components;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Engine)
    Engine() = delete;
    ~Engine() = delete;

    static void SetCamera(const intrusive_ptr<Camera> &camera);

    static unsigned long long GetFrameIndex();
    static float GetDeltaTime();
    static intrusive_ptr<Object2D> GetRoot();
    static intrusive_ptr<Camera> GetCamera();
    static std::shared_ptr<spdlog::logger> GetLogger();
    static std::thread::id &GetMainThreadID();
    static bool IsPlaying();

    static void AddCommand(const pCommand &command);
    static void AddCommand(const std::function<void()> &lambda);
private:
    static void ExecuteCommands();

    static bool Initialize();
    static bool Update();
    static bool PostUpdate();
    static void Pause();
    static void Resume();
    static void Uninitialize();
};

} //namespace a2d

#endif //A2D_ENGINE_H
