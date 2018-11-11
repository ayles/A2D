package com.selya.a2d;

import android.app.Activity;
import android.content.pm.ActivityInfo;

public final class GL2JNI {

    static {
        loadLibrary();
    }

    public static native boolean initialize();
    public static native boolean update();
    public static native void uninitialize();
    public static native void onPause();
    public static native void onResume();
    public static native void resolutionChanged(int width, int height);

    private static void loadLibrary() {
        System.loadLibrary("a2d_android_native");
    }
}
