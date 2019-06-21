package com.example.bullzeyes.imgui_android;

import android.view.Surface;

public class VKViewJniLib {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public static native void init(Surface surface, int surfaceWidth, int surfaceHeight, int windowWidth, int windowHeight, String outputDir);
    public static native void pause();
    public static native void step();
    public static native void destroy();
    public static native void updateTouchEvent(int a, float x, float y, int pointers);
    public static native void updateAccelerometerEvent(long timestamp, float x, float y, float z);
}
