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
#include <cmath>

#include "ImGuiConfigs.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj,  jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight, jstring outputDir);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_pause(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers);
    JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateAccelerometerEvent(JNIEnv * env, jobject obj, jlong timestamp, jfloat x, jfloat y, jfloat z);
};

static bool show_demo_window = false;
static bool show_my_control_panel = true;
static bool initialized = false;
static bool imgGuiShow = true;
static std::string configFile;

static void imgui_Destroy() {

    ImGuiConfigs& configs = ImGuiConfigs::GetInstance();
    configs.SaveToFile(configFile);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
}

static void imgui_Init(int surfaceWidth, int surfaceHeight, int windowWidth, int windowHeight, const char* outputDir) {

    if (initialized) {
        initialized = false;
        imgui_Destroy();
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_InitForOpenGL(surfaceWidth, surfaceHeight, windowWidth, windowHeight, outputDir);
    ImGui_ImplOpenGL3_Init("#version 300 es"); // 300 is implicitly telling imgui_impl_opengl3 to use GLES3

    ImGuiConfigs& configs = ImGuiConfigs::GetInstance();
    configs.LoadFromFile(configFile);

    initialized = true;
}

static void ShowMyControlPanel() {

    static const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(60, 20), ImVec2(720, 1280));

    if (!imgGuiShow) {
        // hide imgui window
        return;
    }

    if (!ImGui::Begin("Control Panel", &imgGuiShow, windowFlags)) {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("Configuration", ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGuiConfigs& configs = ImGuiConfigs::GetInstance();
        ImGui::Checkbox("check me", &configs.checkBoxConfig);
        ImGui::InputInt("input int", &configs.intConfig);
        ImGui::SliderFloat("slider value", &configs.floatSlider.value, configs.floatSlider.min, configs.floatSlider.max);
    }

    ImGui::End();
}

static void imgui_NewFrame() {

    if (!initialized) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    if (show_my_control_panel) {
        ShowMyControlPanel();
    }
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


JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_init(JNIEnv * env, jobject obj, jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight, jstring outputDir)
{
    jboolean isCopy = false;
    const char *nativeString = env->GetStringUTFChars(outputDir, &isCopy);

    configFile = std::string(nativeString).append("/imguiconfigs.json");
    imgui_Init(surfaceWidth, surfaceHeight, windowWidth, windowHeight, nativeString);

    env->ReleaseStringUTFChars(outputDir, nativeString);
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_step(JNIEnv * env, jobject obj)
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    imgui_NewFrame();

    imgui_DrawFrame();
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_pause(JNIEnv * env, jobject obj)
{
    // TODO: consider periodic auto-save
    ImGuiConfigs& configs = ImGuiConfigs::GetInstance();
    configs.SaveToFile(configFile);
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_destroy(JNIEnv * env, jobject obj)
{
    imgui_Destroy();
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers)
{
    imgui_UpdateTouchEvent(action, x, y, pointers);
}

JNIEXPORT void JNICALL Java_com_example_xiaoxing_imgui_1android_GLViewJniLib_updateAccelerometerEvent(JNIEnv * env, jobject obj, jlong timestamp, jfloat x, jfloat y, jfloat z)
{
    static const float Gravity = 9.80665f;
    static const float SHAKE_SPEED_THRESHOLD = 8;

    x = x / Gravity;
    y = y / Gravity;
    z = z / Gravity;

    float speed = sqrt(x * x + y * y + z * z);
    if (speed > SHAKE_SPEED_THRESHOLD) {
        if (!imgGuiShow) {
            imgGuiShow = true;
        }
    }
}