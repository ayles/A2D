//
// Created by selya on 05.11.2018.
//

#include <jni.h>

#include <a2d/core.h>
#include <a2d/core/native_renderer.h>
#include <root_component.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

namespace a2d {

class NativeConnector {
public:
    static bool Initialize() {
        if (!a2d::Engine::Initialize()) return false;
        if (!a2d::NativeRenderer::Initialize()) return false;
        a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
        return true;
    }

    static bool Step() {
        return a2d::Engine::Update() &&
               a2d::Engine::PostUpdate() &&
               a2d::Engine::PreDraw() &&
               a2d::NativeRenderer::Draw() &&
               a2d::Engine::PostDraw();
    }

    static void Uninitialize() {
        a2d::Engine::Uninitialize();
        a2d::NativeRenderer::Uninitialize();
    }

    static void OnPause() {
        a2d::Engine::OnPause();
    }

    static void OnResume() {
        a2d::Engine::OnResume();
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
    a2d::NativeRenderer::ResolutionChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_register_1asset_1manager(JNIEnv *env, jclass type,
                                                   jobject asset_manager) {
    a2d::NativeFileSystem::Initialize(AAssetManager_fromJava(env, asset_manager));
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1touch_1event(JNIEnv *env, jclass type, jobject motion_event) {
    auto clazz = env->GetObjectClass(motion_event);
    auto get_action_masked = env->GetMethodID(clazz, "getActionMasked", "()I");
    auto get_action_index = env->GetMethodID(clazz, "getActionIndex", "()I");
    auto get_pointer_count = env->GetMethodID(clazz, "getPointerCount", "()I");
    auto get_x = env->GetMethodID(clazz, "getX", "()F");
    auto get_y = env->GetMethodID(clazz, "getY", "()F");
    auto action = env->CallIntMethod(motion_event, get_action_masked);
    auto index = env->CallIntMethod(motion_event, get_action_index);
    auto pointer_count = env->CallIntMethod(motion_event, get_pointer_count);
    auto x = env->CallFloatMethod(motion_event, get_x);
    auto y = env->CallFloatMethod(motion_event, get_y);
    a2d::Input::Touch::TouchState state;
    switch (action) {
        case AMOTION_EVENT_ACTION_MOVE:
        case AMOTION_EVENT_ACTION_DOWN:
            state = a2d::Input::Touch::TouchState::PRESS;
            break;
        case AMOTION_EVENT_ACTION_UP:
            state = a2d::Input::Touch::TouchState::RELEASE;
            break;
        default:
            return;
    }
    a2d::Input::SetTouch(pointer_count, index, a2d::Input::Touch {
        state,
        x, y
    });
}

}
