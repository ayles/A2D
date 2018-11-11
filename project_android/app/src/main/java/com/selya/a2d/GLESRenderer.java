package com.selya.a2d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;

public class GLESRenderer implements Renderer {

    private Activity activity;

    public GLESRenderer(Activity activity) {
        this.activity = activity;
    }

    @Override
    public void onDrawFrame(GL10 arg0) {
        GL2JNI.update();
    }

    @Override
    public void onSurfaceChanged(GL10 arg0, int width, int height) {
        GL2JNI.resolutionChanged(width, height);
    }

    @Override
    public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
        GL2JNI.initialize();
    }
}