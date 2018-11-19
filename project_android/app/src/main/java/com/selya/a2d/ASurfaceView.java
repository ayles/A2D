package com.selya.a2d;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class ASurfaceView extends GLSurfaceView {
    public ASurfaceView(Context context) {
        super(context);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        GL2JNI.on_touch_event(event);
        return true;
    }
}
