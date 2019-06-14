#include <jni.h>
#include <string>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include "imgui_impl_android.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_InitForOpenGL();
    ImGui_ImplOpenGL3_Init("#version 300 es"); // 300 is implicitly telling imgui_impl_opengl3 to use GLES3
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj)
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y)
{
    ImGui_ImplAndroid_UpdateTouchEvent(action, x, y);
}