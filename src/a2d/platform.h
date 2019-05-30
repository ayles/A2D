//
// Created by selya on 29.05.2019.
//

#ifndef A2D_PLATFORM_H
#define A2D_PLATFORM_H

#include <a2d/native_to_platform.h>
#include <a2d/platform_to_native.h>

#include <a2d/a2d.hpp>
#include <root_component.hpp>


#if TARGET_ANDROID
#include <android/input.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#endif


#if TARGET_DESKTOP
#include <a2d/renderer/gl.hpp>
#endif



#if TARGET_WINDOWS && FORCE_DISCRETE_GPU
// Enable NVIDIA or AMD discrete gpu on windows
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif


namespace a2d {


#if TARGET_ANDROID
JNIEnv *current_java_env = nullptr;
jclass a2d_bridge_class = nullptr;
#endif


void NativeToPlatform::SetOrientation(Renderer::ScreenOrientation orientation) {
#if TARGET_ANDROID
    int orientation_native = 0;
    switch (orientation) {
        case Renderer::ScreenOrientation::ORIENTATION_LANDSCAPE:
            orientation_native = 0;
            break;
        case Renderer::ScreenOrientation::ORIENTATION_PORTRAIT:
            orientation_native = 1;
            break;
        case Renderer::ScreenOrientation::ORIENTATION_REVERSE_LANDSCAPE:
            orientation_native = 8;
            break;
        case Renderer::ScreenOrientation::ORIENTATION_REVERSE_PORTRAIT:
            orientation_native = 9;
            break;
    }
    if (!current_java_env) { LOG_TRACE("No env"); }
    static auto set_orientation = current_java_env->GetStaticMethodID(a2d_bridge_class, "setOrientation", "(I)V");
    current_java_env->CallStaticVoidMethod(a2d_bridge_class, set_orientation, (jint)orientation_native);
#endif
}


Renderer::ScreenOrientation NativeToPlatform::GetOrientation() {
#if TARGET_ANDROID
    if (!current_java_env) { LOG_TRACE("No env"); }
    static auto get_orientation = current_java_env->GetStaticMethodID(a2d_bridge_class, "getOrientation", "()I");
    switch (current_java_env->CallStaticIntMethod(a2d_bridge_class, get_orientation)) {
        case 0:
            return Renderer::ScreenOrientation::ORIENTATION_LANDSCAPE;
        case 1:
            return Renderer::ScreenOrientation::ORIENTATION_PORTRAIT;
        case 8:
            return Renderer::ScreenOrientation::ORIENTATION_REVERSE_LANDSCAPE;
        case 9:
            return Renderer::ScreenOrientation::ORIENTATION_REVERSE_PORTRAIT;
    }
#endif
    return Renderer::ScreenOrientation::ORIENTATION_LANDSCAPE;
}



bool PlatformToNative::Initialize() {
    if (!Audio::Initialize()) return false;
    if (!Engine::Initialize()) return false;
    if (!Renderer::Initialize()) return false;
    if (!Input::Initialize()) return false;
    if (!Physics::Initialize()) return false;
    Engine::GetRoot()->AddComponent<RootComponent>();
    return true;
}

bool PlatformToNative::Step() {
    Engine::frame_index++;
    // reserve 0 frame index
    if (Engine::frame_index == 0) Engine::frame_index++;

    return Input::Step() &&
           Physics::Step() &&
           Engine::Update() &&
           Engine::PostUpdate() &&
           Renderer::Draw();
}

void PlatformToNative::Uninitialize() {
    Physics::Uninitialize();
    Input::Uninitialize();
    Engine::Uninitialize();
    Renderer::Uninitialize();
    Audio::Uninitialize();
}


#if TARGET_MOBILE
void PlatformToNative::ResolutionChanged(int width, int height, int framebuffer_width, int framebuffer_height) {
    Renderer::ResolutionChanged(width, height, framebuffer_width, framebuffer_height);
}

void PlatformToNative::Pause() {
    Engine::Pause();
}

void PlatformToNative::Resume() {
    Engine::Resume();
}

void PlatformToNative::Touch(int touches_count, int touch_index, a2d::Input::TouchEvent touch_event, float touch_x, float touch_y) {
    Input::TouchCallback(touches_count, touch_index, touch_event, touch_x, touch_y);
}
#endif

#if TARGET_ANDROID
void PlatformToNative::InitializeFileSystem(void *asset_manager) {
    FileSystem::Initialize(asset_manager);
}

void PlatformToNative::SetNativeWindow(EGLNativeWindowType native_window) {
    Renderer::window = native_window;
    Renderer::window_changed = true;
}
#endif
};

#endif // A2D_PLATFORM_H

