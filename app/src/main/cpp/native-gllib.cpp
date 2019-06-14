#include <jni.h>
#include <string>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj)
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj)
{
    ImGui::DestroyContext();
}