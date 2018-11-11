//
// Created by selya on 05.11.2018.
//

#include <jni.h>
#include <android/log.h>

#include <A2D/core.h>
#include <A2D/renderer.h>
#include "native_renderer.h"

#define  LOG_TAG    "a2d_android"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_initialize(JNIEnv *env, jclass type) {
    return (jboolean) (a2d::Engine::Initialize() &&
           a2d::NativeRenderer::Initialize());
}

JNIEXPORT jboolean JNICALL
Java_com_selya_a2d_GL2JNI_update(JNIEnv *env, jclass type) {
    return (jboolean) (a2d::Engine::Update() &&
                       a2d::Engine::PostUpdate() &&
                       a2d::Engine::PreDraw() &&
                       a2d::NativeRenderer::Draw() &&
                       a2d::Engine::PostDraw());
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_uninitialize(JNIEnv *env, jclass type) {
    a2d::NativeRenderer::Uninitialize();
    a2d::Engine::Uninitialize();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_onPause(JNIEnv *env, jclass type) {
    a2d::Engine::OnPause();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_onResume(JNIEnv *env, jclass type) {
    a2d::Engine::OnResume();
}



JNIEXPORT void JNICALL
Java_com_selya_a2d_GL2JNI_resolutionChanged(JNIEnv *env, jclass type, jint width,
                                                          jint height) {
    a2d::NativeRenderer::ResolutionChanged(width, height);
}

}