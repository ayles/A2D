package com.selya.a2d;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.view.MotionEvent;

public final class GL2JNI {

    static {
        loadLibrary();
    }

    public static native boolean initialize();
    public static native boolean on_surface_created();
    public static native boolean on_draw_frame();
    public static native void on_pause();
    public static native void on_resume();
    public static native void on_surface_changed(int width, int height);
    public static native void on_destroy();
    public static native void register_asset_manager(AssetManager asset_manager);
    public static native void on_touch_event(MotionEvent motion_event);

    private static void loadLibrary() {
        System.loadLibrary("a2d_android_library");
    }
}
