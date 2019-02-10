//
// Created by selya on 26.10.2018.
//

#include <a2d/core/engine.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/camera.hpp>
#include <a2d/core/log.hpp>

unsigned long long a2d::Engine::frame_index = 0;
float a2d::Engine::delta_time = 0.0f;
a2d::pObject2D a2d::Engine::root = nullptr;
a2d::pObject2D a2d::Engine::gui_root = nullptr;
a2d::pCamera a2d::Engine::camera = nullptr;
std::thread::id a2d::Engine::main_thread_id;
bool a2d::Engine::playing = false;
std::queue<a2d::pCommand> a2d::Engine::commands;
std::set<a2d::pComponent> a2d::Engine::components;


void a2d::Engine::SetCamera(const a2d::pCamera &camera) {
    ASSERT_MAIN_THREAD
    if (!camera) { LOG_TRACE("Camera is null"); }
    Engine::camera = camera;
}

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

a2d::pObject2D a2d::Engine::GetGUIRoot() {
    ASSERT_MAIN_THREAD
    return gui_root;
}

a2d::pCamera a2d::Engine::GetCamera() {
    ASSERT_MAIN_THREAD
    return camera;
}

std::thread::id &a2d::Engine::GetMainThreadID() {
    return main_thread_id;
}

bool a2d::Engine::IsPlaying() {
    return playing;
}


bool a2d::Engine::Initialize() {
    main_thread_id = std::this_thread::get_id();
    root = new Object2D;
    gui_root = new Object2D;

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
