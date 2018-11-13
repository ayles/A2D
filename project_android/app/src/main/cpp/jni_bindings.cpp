//
// Created by selya on 05.11.2018.
//

#include <jni.h>

#include <a2d/core.h>
#include <a2d/core/native_renderer.h>
#include <root_component.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_initialize(JNIEnv *env, jclass type) {
    if (!a2d::Engine::Initialize()) return JNI_FALSE;
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_on_1surface_1created(JNIEnv *env, jclass type) {
    if (!a2d::NativeRenderer::Initialize()) return JNI_FALSE;
    a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_on_1draw_1frame(JNIEnv *env, jclass type) {
    return (jboolean) (a2d::Engine::Update() &&
                       a2d::Engine::PostUpdate() &&
                       a2d::Engine::PreDraw() &&
                       a2d::NativeRenderer::Draw() &&
                       a2d::Engine::PostDraw());
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1destroy(JNIEnv *env, jclass type) {
    a2d::NativeRenderer::Uninitialize();
    a2d::Engine::Uninitialize();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1pause(JNIEnv *env, jclass type) {
    a2d::Engine::OnPause();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_on_1resume(JNIEnv *env, jclass type) {
    a2d::Engine::OnResume();
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

}
