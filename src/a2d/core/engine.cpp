//
// Created by selya on 26.10.2018.
//

#include <a2d/core/engine.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/command.hpp>
#include <a2d/core/component.hpp>
#include <a2d/core/log.hpp>
#include <a2d/core/commands/lambda_command.hpp>

namespace a2d {

unsigned long long Engine::frame_index = 0;
std::chrono::time_point<std::chrono::system_clock> Engine::frame_start_time;
float Engine::delta_time = 0.0f;
intrusive_ptr<Object2D> Engine::root = nullptr;
std::thread::id Engine::main_thread_id;
bool Engine::playing = false;
std::queue<intrusive_ptr<Command>> Engine::commands;
std::set<intrusive_ptr<Component>> Engine::components;

unsigned long long Engine::GetFrameIndex() {
    return frame_index;
}

float Engine::GetDeltaTime() {
    return delta_time;
}

intrusive_ptr<Object2D> Engine::GetRoot() {
    return root;
}

const std::thread::id &Engine::GetMainThreadID() {
    return main_thread_id;
}

bool Engine::IsPlaying() {
    return playing;
}


bool Engine::Initialize() {
    main_thread_id = std::this_thread::get_id();
    root = new Object2D;
    Resume();
    return true;
}

bool Engine::Update() {
    delta_time = std::chrono::duration<float>(std::chrono::system_clock::now() - frame_start_time).count();
    frame_start_time = std::chrono::system_clock::now();

    for (auto &component : components) {
        if (!component->IsActiveTransitive()) continue;
        component->Update();
    }
    ExecuteCommands();

    return true;
}

bool Engine::PostUpdate() {
    for (auto &component : components) {
        if (!component->IsActiveTransitive()) continue;
        component->PostUpdate();
    }
    ExecuteCommands();
    return true;
}

void Engine::Pause() {
    if (!playing) return;
    playing = false;
    for (auto &component : components) component->OnPause();
}

void Engine::Resume() {
    if (playing) return;
    playing = true;
    frame_start_time = std::chrono::system_clock::now();
    for (auto &component : components) component->OnResume();
}

void Engine::Uninitialize() {
    Pause();
    root->Destroy();
}

void Engine::AddCommand(const intrusive_ptr<Command> &command) {
    commands.emplace(command);
}

void Engine::AddCommand(const std::function<void()> &lambda) {
    commands.emplace(new LambdaCommand(lambda));
}

void Engine::ExecuteCommands() {
    while (!commands.empty()) {
        commands.front()->Execute();
        commands.pop();
    }
}

}