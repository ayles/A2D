package com.selya.a2d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;

public class GLESRenderer implements Renderer {

    @Override
    public void onDrawFrame(GL10 arg0) {
        A2DBridge.onDrawFrame();
    }

    @Override
    public void onSurfaceChanged(GL10 arg0, int width, int height) {
        A2DBridge.onSurfaceChanged(width, height);
    }

    @Override
    public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
        A2DBridge.onSurfaceCreated();
    }
}