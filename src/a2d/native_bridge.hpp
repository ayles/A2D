//
// Created by selya on 24.01.2019.
//

#ifndef A2D_BRIDGE_H
#define A2D_BRIDGE_H

#include <a2d/a2d.hpp>
#include <root_component.hpp>

#ifdef TARGET_ANDROID
#include <jni.h>
#include <android/input.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif


#if defined(TARGET_WINDOWS) && defined(FORCE_DISCRETE_GPU)
// Enable NVIDIA or AMD discrete gpu on windows
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
};
#endif

namespace a2d {

class NativeBridge {
public:
    static bool Initialize() {
        if (!Audio::Initialize()) return false;
        if (!Engine::Initialize()) return false;
        if (!Renderer::Initialize()) return false;
        if (!Input::Initialize()) return false;
        if (!Physics::Initialize()) return false;
        Engine::GetRoot()->AddComponent<RootComponent>();
        return true;
    }

    static bool Step() {
        return Physics::Step() &&
               Engine::Update() &&
               Engine::PostUpdate() &&
               Renderer::Draw();
    }

    static void Uninitialize() {
        Physics::Uninitialize();
        Input::Uninitialize();
        Engine::Uninitialize();
        Renderer::Uninitialize();
        Audio::Uninitialize();
    }

#ifdef TARGET_ANDROID
    static void ResolutionChanged(int width, int height) {
        Renderer::ResolutionChanged(width, height);
    }

    static void OnPause() {
        Engine::Pause();
    }

    static void OnResume() {
        Engine::Resume();
    }

    static void OnTouch(int touches_count, int touch_index, int touch_action, float touch_x, float touch_y) {
        Input::TouchEvent touch_event;
        switch (touch_action) {
            case AMOTION_EVENT_ACTION_DOWN: touch_event = Input::TouchEvent::TOUCH_BEGAN; break;
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_CANCEL: touch_event = Input::TouchEvent::TOUCH_ENDED; break;
            case AMOTION_EVENT_ACTION_MOVE: touch_event = Input::TouchEvent::TOUCH_MOVED; break;

        }
        Input::TouchCallback(touches_count, touch_index, touch_event, touch_x, touch_y);
    }

    static void InitializeFS(void *asset_manager) {
        FileSystem::Initialize(asset_manager);
    }
#endif
};

}

#ifdef TARGET_ANDROID

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_A2DBridge_onSurfaceCreated(JNIEnv *env, jclass type) {
    if (!a2d::NativeBridge::Initialize()) return JNI_FALSE;
    return JNI_TRUE;
}

JNIEnv *current_env = nullptr;
jclass a2d_bridge = nullptr;
JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_A2DBridge_onDrawFrame(JNIEnv *env, jclass type) {
    current_env = env;
    a2d_bridge = type;
    jboolean state = (jboolean)a2d::NativeBridge::Step();
    a2d_bridge = nullptr;
    current_env = nullptr;
    return state;
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_onDestroy(JNIEnv *env, jclass type) {
    a2d::NativeBridge::Uninitialize();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_onPause(JNIEnv *env, jclass type) {
    a2d::NativeBridge::OnPause();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_onResume(JNIEnv *env, jclass type) {
    a2d::NativeBridge::OnResume();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_onSurfaceChanged(JNIEnv *env, jclass type, jint width,
                                              jint height) {
    a2d::NativeBridge::ResolutionChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_registerAssetManager(JNIEnv *env, jclass type,
                                                  jobject asset_manager) {
    a2d::NativeBridge::InitializeFS(AAssetManager_fromJava(env, asset_manager));
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_onTouchEvent(JNIEnv *env, jclass type, jobject motion_event) {
    static auto clazz = env->GetObjectClass(motion_event);
    static auto get_action_masked = env->GetMethodID(clazz, "getActionMasked", "()I");
    static auto get_action_index = env->GetMethodID(clazz, "getActionIndex", "()I");
    static auto get_pointer_count = env->GetMethodID(clazz, "getPointerCount", "()I");
    static auto get_x = env->GetMethodID(clazz, "getX", "()F");
    static auto get_y = env->GetMethodID(clazz, "getY", "()F");

    auto action = env->CallIntMethod(motion_event, get_action_masked);
    auto index = env->CallIntMethod(motion_event, get_action_index);
    auto pointer_count = env->CallIntMethod(motion_event, get_pointer_count);
    auto x = env->CallFloatMethod(motion_event, get_x);
    auto y = env->CallFloatMethod(motion_event, get_y);

    a2d::NativeBridge::OnTouch(pointer_count, index, action, x, y);
}

}

void Android_setOrientation(int orientation) {
    ASSERT_MAIN_THREAD
    if (!current_env) { LOG_TRACE("No env"); }
    static auto set_orientation = current_env->GetStaticMethodID(a2d_bridge, "setOrientation", "(I)V");
    current_env->CallStaticVoidMethod(a2d_bridge, set_orientation, (jint)orientation);
}

int Android_getOrientation() {
    ASSERT_MAIN_THREAD
    if (!current_env) { LOG_TRACE("No env"); }
    static auto get_orientation = current_env->GetStaticMethodID(a2d_bridge, "getOrientation", "()I");
    return current_env->CallStaticIntMethod(a2d_bridge, get_orientation);
}

#endif

#endif //A2D_BRIDGE_H
