//
// Created by selya on 05.11.2018.
//

#include <jni.h>

#include <a2d/core.h>
#include <a2d/core/renderer.h>
#include <root_component.hpp>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

namespace a2d {

class NativeConnector {
public:
    static bool Initialize() {
        if (!a2d::Audio::Initialize()) return false;
        if (!a2d::Engine::Initialize()) return false;
        if (!a2d::Renderer::Initialize()) return false;
        if (!a2d::Input::Initialize()) return false;
        a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
        return true;
    }

    static bool Step() {
        return a2d::Engine::Update() &&
               a2d::Engine::PostUpdate() &&
               a2d::Engine::PreDraw() &&
               a2d::Renderer::Draw() &&
               a2d::Engine::PostDraw();
    }

    static void Uninitialize() {
        a2d::Input::Uninitialize();
        a2d::Engine::Uninitialize();
        a2d::Renderer::Uninitialize();
        a2d::Audio::Uninitialize();
        a2d::FileSystem::Uninitialize();
    }

    static void ResolutionChanged(int width, int height) {
        a2d::Renderer::ResolutionChanged(width, height);
    }

    static void OnPause() {
        a2d::Engine::OnPause();
    }

    static void OnResume() {
        a2d::Engine::OnResume();
    }

    static void OnTouch(int touches_count, int touch_index, int touch_action, float touch_x, float touch_y) {
        a2d::Input::TouchCallback(touches_count, touch_index, touch_action, touch_x, touch_y);
    }
};

} //namespace a2d

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_initialize(JNIEnv *env, jclass type) {
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_on_1surface_1created(JNIEnv *env, jclass type) {
    if (!a2d::NativeConnector::Initialize()) return JNI_FALSE;
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_on_1draw_1frame(JNIEnv *env, jclass type) {
    return (jboolean) (a2d::NativeConnector::Step());
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1destroy(JNIEnv *env, jclass type) {
    a2d::NativeConnector::Uninitialize();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1pause(JNIEnv *env, jclass type) {
    a2d::NativeConnector::OnPause();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1resume(JNIEnv *env, jclass type) {
    a2d::NativeConnector::OnResume();
}



JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1surface_1changed(JNIEnv *env, jclass type, jint width,
                                                          jint height) {
    a2d::NativeConnector::ResolutionChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_register_1asset_1manager(JNIEnv *env, jclass type,
                                                   jobject asset_manager) {
    a2d::FileSystem::Initialize(AAssetManager_fromJava(env, asset_manager));
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1touch_1event(JNIEnv *env, jclass type, jobject motion_event) {
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

    a2d::NativeConnector::OnTouch(pointer_count, index, action, x, y);
}

}
