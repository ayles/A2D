//
// Created by selya on 26.10.2018.
//

#include <a2d/core/engine.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/components/camera.hpp>

#ifdef TARGET_ANDROID
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_sinks.h>
#endif

#include <chrono>
#include "engine.hpp"


unsigned long long a2d::Engine::frame_index = 0;
float a2d::Engine::delta_time = 0.0f;
a2d::pObject2D a2d::Engine::root = new Object2D;
a2d::pCamera a2d::Engine::camera = nullptr;
std::shared_ptr<spdlog::logger> a2d::Engine::logger = nullptr;
std::thread::id a2d::Engine::ui_thread_id;
bool a2d::Engine::playing = false;
std::queue<a2d::pCommand> a2d::Engine::commands;
std::list<a2d::pComponent> a2d::Engine::components;


void a2d::Engine::SetCamera(const a2d::pCamera &camera) {
    Engine::camera = camera;
}

unsigned long long a2d::Engine::GetFrameIndex() {
    return frame_index;
}

float a2d::Engine::GetDeltaTime() {
    return delta_time;
}

a2d::pObject2D a2d::Engine::GetRoot() {
    return root;
}

a2d::pCamera a2d::Engine::GetCamera() {
    return camera;
}

std::shared_ptr<spdlog::logger> a2d::Engine::GetLogger() {
    return logger;
}

std::thread::id &a2d::Engine::GetUIThreadID() {
    return ui_thread_id;
}

bool a2d::Engine::IsPlaying() {
    return playing;
}


bool a2d::Engine::Initialize() {
    ui_thread_id = std::this_thread::get_id();

#ifdef TARGET_ANDROID
    logger = spdlog::android_logger_mt("logger", "a2d_log");
#else
    logger = spdlog::stdout_logger_mt("logger");
#endif

    logger->set_level(spdlog::level::info);
    logger->set_pattern("%+");

    Resume();

    return true;
}

bool a2d::Engine::Update() {
    static auto start = std::chrono::system_clock::now();

    frame_index++;
    // reserve 0 frame index
    if (frame_index == 0) frame_index++;

    delta_time = std::chrono::duration<float>(std::chrono::system_clock::now() - start).count();
    start = std::chrono::system_clock::now();

#ifdef TARGET_DESKTOP
    // For fast input handling
    glfwPollEvents();
#endif

    for (auto &component : components) {
        component->Update();
    }
    ExecuteCommands();

    return true;
}

bool a2d::Engine::PostUpdate() {
    for (auto &component : components) {
        component->PostUpdate();
    }
    ExecuteCommands();
    return true;
}

void a2d::Engine::Pause() {
    if (!playing) return;
    playing = false;
    for (auto &component : components) {
        component->OnPause();
    }
}

void a2d::Engine::Resume() {
    if (playing) return;
    playing = true;
    for (auto &component : components) {
        component->OnResume();
    }
}

void a2d::Engine::Uninitialize() {
    Pause();
    root->CleanTree();
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
