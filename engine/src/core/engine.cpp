//
// Created by selya on 26.10.2018.
//

#include <a2d/core/renderer.h>
#include <a2d/core/engine.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/camera.h>

#ifdef TARGET_ANDROID
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_sinks.h>
#endif

#include <chrono>
#include <cmath>

float a2d::Engine::delta_time = 0.0f;
a2d::pObject2D a2d::Engine::root = new Object2D;
a2d::pCamera a2d::Engine::camera = nullptr;
std::shared_ptr<spdlog::logger> a2d::Engine::logger = nullptr;
std::thread::id a2d::Engine::ui_thread_id;
bool a2d::Engine::playing = false;

bool a2d::Engine::Initialize() {
    ui_thread_id = std::this_thread::get_id();
    // Important for hierarchical activate/disable
    root->is_active = true;

#ifdef TARGET_ANDROID
    logger = spdlog::android_logger_mt("logger", "a2d_log");
#else
    logger = spdlog::stdout_logger_mt("logger");
#endif

    logger->set_level(spdlog::level::info);
    logger->set_pattern("%+");

    OnResume();

    return true;
}

bool a2d::Engine::Update() {
    static auto start = std::chrono::system_clock::now();

    delta_time = std::chrono::duration<float>(std::chrono::system_clock::now() - start).count();
    start = std::chrono::system_clock::now();

    root->Update();

    return true;
}

bool a2d::Engine::PostUpdate() {
    root->PostUpdate();
    return true;
}

bool a2d::Engine::PreDraw() {
    root->PreDraw(Matrix4f());
    return true;
}

bool a2d::Engine::PostDraw() {
    root->PostDraw();
    return true;
}

void a2d::Engine::OnPause() {
    if (!playing) return;
    playing = false;
    root->OnPause();
}

void a2d::Engine::OnResume() {
    if (playing) return;
    playing = true;
    root->OnResume();
}

void a2d::Engine::Uninitialize() {
    OnPause();
    root->CleanTree();
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

void a2d::Engine::SetCamera(a2d::pCamera camera) {
    Engine::camera = camera;
}

std::shared_ptr<spdlog::logger> a2d::Engine::GetLogger() {
    return logger;
}

std::thread::id a2d::Engine::GetUIThreadID() {
    return ui_thread_id;
}

bool a2d::Engine::IsPlaying() {
    return playing;
}