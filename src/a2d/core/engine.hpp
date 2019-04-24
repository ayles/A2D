//
// Created by selya on 26.10.2018.
//

#ifndef A2D_ENGINE_HPP
#define A2D_ENGINE_HPP

#include <a2d/core/macro.hpp>

#include <thread>
#include <queue>
#include <set>
#include <cassert>

#define ASSERT_MAIN_THREAD assert(std::this_thread::get_id() == a2d::Engine::GetMainThreadID());

namespace a2d {

class Object2D;
class Component;
class Command;

class Engine {
    friend class Renderer;
    friend class NativeBridge;
    friend class Component;
    friend class Object2D;
    friend class Physics;

    static unsigned long long frame_index;
    static float delta_time;
    static intrusive_ptr<Object2D> root;
    static std::thread::id main_thread_id;
    static bool playing;
    static std::queue<intrusive_ptr<Command>> commands;
    static std::set<intrusive_ptr<Component>> components;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Engine)
    Engine() = delete;
    ~Engine() = delete;

    static unsigned long long GetFrameIndex();
    static float GetDeltaTime();
    static intrusive_ptr<Object2D> GetRoot();
    static const std::thread::id &GetMainThreadID();
    static bool IsPlaying();

private:
    static void AddCommand(const intrusive_ptr<Command> &command);
    static void AddCommand(const std::function<void()> &lambda);
    static void ExecuteCommands();

    static bool Initialize();
    static bool Update();
    static bool PostUpdate();
    static void Pause();
    static void Resume();
    static void Uninitialize();
};

} //namespace a2d

#endif //A2D_ENGINE_HPP
