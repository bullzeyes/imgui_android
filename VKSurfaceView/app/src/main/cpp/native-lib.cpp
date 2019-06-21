#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <vector>
#include "VulkanMain.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "imgui_impl_android", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "imgui_impl_android", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "imgui_impl_android", __VA_ARGS__))

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_init(JNIEnv * env, jobject obj, jobject surface, jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight, jstring outputDir);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_pause(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateAccelerometerEvent(JNIEnv * env, jobject obj, jlong timestamp, jfloat x, jfloat y, jfloat z);
};


JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_init(JNIEnv * env, jobject obj,  jobject surface, jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight, jstring outputDir)
{
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);

    if (IsVulkanReady()) {
        DeleteVulkan();
    }

    InitVulkan(nativeWindow);
}


JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_pause(JNIEnv * env, jobject obj)
{

}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_step(JNIEnv * env, jobject obj)
{
    if (IsVulkanReady()) {
        VulkanDrawFrame();
    }
}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_destroy(JNIEnv * env, jobject obj)
{
    if (IsVulkanReady()) {
        DeleteVulkan();
    }
}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers)
{

}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateAccelerometerEvent(JNIEnv * env, jobject obj, jlong timestamp, jfloat x, jfloat y, jfloat z)
{

}
