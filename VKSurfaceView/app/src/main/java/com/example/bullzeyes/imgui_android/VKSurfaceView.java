package com.example.bullzeyes.imgui_android;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class VKSurfaceView extends SurfaceView implements SurfaceHolder.Callback2 {

    private final static String TAG = "VKSurfaceView";

    /**
     * Standard View constructor.
     */
    public VKSurfaceView(Context context) {
        super(context);
        init();
    }

    /**
     * Standard View constructor.
     */
    public VKSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed
        SurfaceHolder holder = getHolder();
        holder.addCallback(this);

        mRenderThread = new RenderThread();
        mRenderThread.start();
    }

    /**
     * This method is part of the SurfaceHolder.Callback interface, and is
     * not normally called or subclassed by clients of GLSurfaceView.
     */
    public void surfaceCreated(SurfaceHolder holder) {

    }

    /**
     * This method is part of the SurfaceHolder.Callback interface, and is
     * not normally called or subclassed by clients of GLSurfaceView.
     */
    public void surfaceDestroyed(SurfaceHolder holder) {
        VKViewJniLib.destroy();
    }

    /**
     * This method is part of the SurfaceHolder.Callback interface, and is
     * not normally called or subclassed by clients of GLSurfaceView.
     */
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

        Log.e("XXX", String.format("surfaceChanged: %d, %d", w, h));
        VKViewJniLib.init(holder.getSurface(), w, h, w, h, null);
    }

    public void surfaceRedrawNeeded(SurfaceHolder holder) {
        // Since we are part of the framework we know only surfaceRedrawNeededAsync
        // will be called.
    }

    static class RenderThread extends Thread {

        @Override
        public void run() {
            setName("GLThread " + getId());
            try {
                while (true) {
                    if (mShouldExit) {
                        return;
                    }

                    VKViewJniLib.step();
                }
            }
            finally {

            }
        }

        private boolean mShouldExit = false;

    }

    private RenderThread mRenderThread;
}
