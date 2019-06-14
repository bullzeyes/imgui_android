package com.example.xiaoxing.imgui_android;

public class GLViewJniLib {

    static {
        System.loadLibrary("native-gllib");
    }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);
    public static native void step();
    public static native void destroy();
    public static native void updateTouchEvent(int a, float x, float y);
}
