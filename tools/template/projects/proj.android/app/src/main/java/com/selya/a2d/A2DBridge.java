package com.selya.a2d;

import android.arch.core.util.Function;
import android.content.res.AssetManager;
import android.support.v4.util.Consumer;
import android.view.MotionEvent;

public final class A2DBridge {

    static {
        loadLibrary();
    }

    @FunctionalInterface
    interface Supplier<T> {
        T get();
    }

    public static Consumer<Integer> setOrientationCallback = null;
    public static Supplier<Integer> getOrientationCallback = null;

    public static native boolean onSurfaceCreated();
    public static native boolean onDrawFrame();
    public static native void onPause();
    public static native void onResume();
    public static native void onSurfaceChanged(int width, int height);
    public static native void onDestroy();
    public static native void registerAssetManager(AssetManager asset_manager);
    public static native void onTouchEvent(MotionEvent motion_event);

    public static void setOrientation(int orientation) {
        if (setOrientationCallback != null) {
            setOrientationCallback.accept(orientation);
        }
    }

    public static int getOrientation() {
        if (getOrientationCallback != null) {
            return getOrientationCallback.get();
        }
        return 0;
    }

    private static void loadLibrary() {
        System.loadLibrary("a2d_android_library");
    }
}
