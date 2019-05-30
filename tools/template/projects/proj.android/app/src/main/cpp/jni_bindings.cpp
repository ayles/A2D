//
// Created by selya on 05.11.2018.
//

#include <a2d/a2d.hpp>
#include <a2d/platform.h>
#include <jni.h>
#include <android/input.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>

#include <queue>
#include <functional>

extern "C" {

static std::queue<std::function<void()>> command_queue;
static std::mutex mutex;
static std::condition_variable step_lock;
static bool pause_loop = false;
static bool should_terminate = false;

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_start(JNIEnv *env, jclass type, jobject surface, jobject asset_manager) {
a2d::current_java_env = env;
a2d::a2d_bridge_class = type;
a2d::PlatformToNative::InitializeFileSystem(AAssetManager_fromJava(env, asset_manager));
a2d::PlatformToNative::SetNativeWindow(
(EGLNativeWindowType) ANativeWindow_fromSurface(env, surface));
a2d::PlatformToNative::Initialize();
while (!should_terminate && a2d::PlatformToNative::Step()) {
std::unique_lock<std::mutex> l(mutex);
while (!command_queue.empty()) {
command_queue.front()();
command_queue.pop();
}
while (pause_loop && !should_terminate) step_lock.wait(l);
}
a2d::PlatformToNative::Uninitialize();
a2d::current_java_env = nullptr;
a2d::a2d_bridge_class = nullptr;
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_terminate(JNIEnv *env, jclass type) {
should_terminate = true;
step_lock.notify_all();
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_pause(JNIEnv *env, jclass type) {
command_queue.push([]() {
a2d::PlatformToNative::Pause();
pause_loop = true;
});
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_resume(JNIEnv *env, jclass type) {
command_queue.push([]() {
a2d::PlatformToNative::Resume();
pause_loop = false;
step_lock.notify_all();
});
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_setSurface(JNIEnv *env, jclass type, jobject surface) {
static EGLNativeWindowType window = nullptr;
if (surface) {
window = (EGLNativeWindowType) ANativeWindow_fromSurface(env, surface);
} else {
ANativeWindow_release(window);
window = nullptr;
}
command_queue.push([]() {
a2d::PlatformToNative::SetNativeWindow(window);
});
if (window) {
pause_loop = false;
step_lock.notify_all();
} else {
pause_loop = true;
}
}

JNIEXPORT void JNICALL
Java_com_selya_a2d_A2DBridge_sendTouchEvent(JNIEnv *env, jclass type, jobject motion_event) {
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

a2d::Input::TouchEvent touch_event;
switch (action) {
case AMOTION_EVENT_ACTION_DOWN:
touch_event = a2d::Input::TouchEvent::TOUCH_BEGAN;
break;
case AMOTION_EVENT_ACTION_UP:
case AMOTION_EVENT_ACTION_CANCEL:
touch_event = a2d::Input::TouchEvent::TOUCH_ENDED;
break;
case AMOTION_EVENT_ACTION_MOVE:
touch_event = a2d::Input::TouchEvent::TOUCH_MOVED;
break;
}

command_queue.push([pointer_count, index, touch_event, x, y]() {
a2d::PlatformToNative::Touch(pointer_count, index, touch_event, x, y);
});
}

}