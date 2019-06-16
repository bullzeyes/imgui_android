#include <jni.h>
#include <string>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include "imgui_impl_android.h"

#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj,  jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers);
};

static bool initialized = false;

static void imgui_Destroy() {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
}

static void imgui_Init(int surfaceWidth, int surfaceHeight, int windowWidth, int windowHeight) {

    if (initialized) {
        initialized = false;
        imgui_Destroy();
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_InitForOpenGL(surfaceWidth, surfaceHeight, windowWidth, windowHeight);
    ImGui_ImplOpenGL3_Init("#version 300 es"); // 300 is implicitly telling imgui_impl_opengl3 to use GLES3

    initialized = true;
}

static void imgui_NewFrame() {

    if (!initialized) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
}

static void imgui_DrawFrame() {

    if (!initialized) {
        return;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void imgui_UpdateTouchEvent(int action, float x, float y, int pointers) {

    if (!initialized) {
        return;
    }

    ImGui_ImplAndroid_UpdateTouchEvent(action, x, y, pointers);
}


JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj, jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight)
{
    imgui_Init(surfaceWidth, surfaceHeight, windowWidth, windowHeight);
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj)
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    imgui_NewFrame();

    imgui_DrawFrame();
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj)
{
    imgui_Destroy();
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers)
{
    imgui_UpdateTouchEvent(action, x, y, pointers);
}