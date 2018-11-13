package com.selya.a2d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;

public class GLESRenderer implements Renderer {

    @Override
    public void onDrawFrame(GL10 arg0) {
        GL2JNI.on_draw_frame();
    }

    @Override
    public void onSurfaceChanged(GL10 arg0, int width, int height) {
        GL2JNI.on_surface_changed(width, height);
    }

    @Override
    public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
        GL2JNI.on_surface_created();
    }
}