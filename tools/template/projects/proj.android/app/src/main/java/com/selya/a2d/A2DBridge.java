package com.selya.a2d;

import android.content.res.AssetManager;
import android.support.v4.util.Consumer;
import android.view.MotionEvent;
import android.view.Surface;

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

    public static native void start(Surface surface, AssetManager assetManager);
    public static native void terminate();
    public static native void pause();
    public static native void resume();
    public static native void setSurface(Surface surface);
    public static native void sendTouchEvent(MotionEvent motionEvent);

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
