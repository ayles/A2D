package com.selya.a2d;

import android.content.Context;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ASurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private Thread renderThread;

    public ASurfaceView(Context context) {
        super(context);
        getHolder().addCallback(this);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        A2DBridge.sendTouchEvent(event);
        return true;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        System.out.println("----------Created " + holder.getSurface());
        if (renderThread == null) {
            renderThread = new Thread(() -> A2DBridge.start(holder.getSurface(), getResources().getAssets()));
            renderThread.start();
        } else {
            A2DBridge.setSurface(getHolder().getSurface());
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        System.out.println("----------Changed " + holder.getSurface() + " " + width + " " + height);
        A2DBridge.setSurface(getHolder().getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        System.out.println("----------Destroyed " + holder.getSurface());
        A2DBridge.setSurface(null);
    }
}
