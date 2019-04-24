//
// Created by selya on 26.10.2018.
//

#include <a2d/core/engine.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/component.hpp>
#include <a2d/core/log.hpp>

unsigned long long a2d::Engine::frame_index = 0;
float a2d::Engine::delta_time = 0.0f;
a2d::pObject2D a2d::Engine::root = nullptr;
std::thread::id a2d::Engine::main_thread_id;
bool a2d::Engine::playing = false;
std::queue<a2d::pCommand> a2d::Engine::commands;
std::set<a2d::pComponent> a2d::Engine::components;

unsigned long long a2d::Engine::GetFrameIndex() {
    return frame_index;
}

float a2d::Engine::GetDeltaTime() {
    return delta_time;
}

a2d::pObject2D a2d::Engine::GetRoot() {
    ASSERT_MAIN_THREAD
    return root;
}

const std::thread::id &a2d::Engine::GetMainThreadID() {
    return main_thread_id;
}

bool a2d::Engine::IsPlaying() {
    return playing;
}


bool a2d::Engine::Initialize() {
    main_thread_id = std::this_thread::get_id();
    root = new Object2D;
    Resume();
    return true;
}

bool a2d::Engine::Update() {
    static auto start = std::chrono::system_clock::now();

    delta_time = std::chrono::duration<float>(std::chrono::system_clock::now() - start).count();
    start = std::chrono::system_clock::now();

    for (auto &component : components) {
        if (!component->IsActiveTransitive()) continue;
        component->Update();
    }
    ExecuteCommands();

    return true;
}

bool a2d::Engine::PostUpdate() {
    for (auto &component : components) {
        if (!component->IsActiveTransitive()) continue;
        component->PostUpdate();
    }
    ExecuteCommands();
    return true;
}

void a2d::Engine::Pause() {
    if (!playing) return;
    playing = false;
    for (auto &component : components) component->OnPause();
}

void a2d::Engine::Resume() {
    if (playing) return;
    playing = true;
    for (auto &component : components) component->OnResume();
}

void a2d::Engine::Uninitialize() {
    Pause();
    root->Destroy();
}

void a2d::Engine::AddCommand(const a2d::pCommand &command) {
    commands.emplace(command);
}

void a2d::Engine::AddCommand(const std::function<void()> &lambda) {
    commands.emplace(new LambdaCommand(lambda));
}

void a2d::Engine::ExecuteCommands() {
    while (!commands.empty()) {
        commands.front()->Execute();
        commands.pop();
    }
}
