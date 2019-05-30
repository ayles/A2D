//
// Created by selya on 29.05.2019.
//

#ifndef A2D_PLATFORM_TO_NATIVE_H
#define A2D_PLATFORM_TO_NATIVE_H

namespace a2d {

class PlatformToNative {
public:
    static bool Initialize();
    static bool Step();
    static void Uninitialize();

#if TARGET_MOBILE
    static void ResolutionChanged(int width, int height, int framebuffer_width = -1, int framebuffer_height = -1);
    static void Pause();
    static void Resume();
    static void Touch(int touches_count, int touch_index, a2d::Input::TouchEvent touch_event, float touch_x, float touch_y);
#endif

#if TARGET_ANDROID
    static void InitializeFileSystem(void *asset_manager);
    static void SetNativeWindow(EGLNativeWindowType native_window);
#endif
};

}

#endif //A2D_PLATFORM_TO_NATIVE_H
