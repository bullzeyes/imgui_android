//
// Created by Xiaoxing Zhu on 6/13/19.
//

#include <imgui/imgui.h>
#include <bits/timespec.h>
#include <time.h>
#include "imgui/imgui.h"
#include "imgui_impl_android.h"

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "imgui_impl_android", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "imgui_impl_android", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "imgui_impl_android", __VA_ARGS__))

static float g_Time; // time elapse in seconds
static TOUCH_EVENT g_LastTouchEvent;
static int g_SurfaceWidth, g_SurfaceHeight;

long get_time_us()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000 + (now.tv_nsec / 1000);
}

bool ImGui_ImplAndroid_InitForOpenGL(int width, int height)
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "imgui_impl_android";
    g_SurfaceWidth = width;
    g_SurfaceHeight = height;
    return true;
}

bool ImGui_ImplAndroid_InitForVulkan(int width, int height)
{
    return true;
}

void ImGui_ImplAndroid_Shutdown()
{

}

void ImGui_ImplAndroid_UpdateTouchEvent(int a, float x, float y, int pointers)
{
    // TODO: Synchronization issue, potentially drop events
    if (a < TOUCH_ACTION_COUNT) {
        float velocity_y = (float)(y - g_LastTouchEvent.y) / 100.f;
        g_LastTouchEvent = {.action = (TOUCH_ACTION) a, .x = x, .y = y, .pointers = pointers, .y_velocity = velocity_y};
    }
    else {
        LOGE("invalid action index: %d", a);
    }
}

static void ImGui_ImplAndroid_UpdateMousePosAndButtons() {
    // Update buttons
    ImGuiIO &io = ImGui::GetIO();

    // Mouse buttons: 0=left, 1=right, 2=middle + extras. ImGui itself mostly only uses left button
    // (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse
    // is being used by your application + available to user as a convenience via IsMouse** API.
    // TOUCH_ACTION_MOVE -> MousePos
    // TOUCH_ACTION_DOWN -> MouseDown[0] true, left button
    // TOUCH_ACTION_UP -> MouseDown[0] false. left button
    // TOUCH_ACTION_POINTER_DOWN -> multi finger as scroll, set MouseWheel. MouseWheelH not used
    if (TOUCH_ACTION_DOWN <= g_LastTouchEvent.action &&
        g_LastTouchEvent.action <= TOUCH_ACTION_MOVE) {
        io.MousePos.x = g_LastTouchEvent.x;
        io.MousePos.y = g_LastTouchEvent.y;
    }
    switch (g_LastTouchEvent.action) {
        case TOUCH_ACTION_MOVE:
            if (g_LastTouchEvent.pointers > 1) {
                io.MouseWheel = g_LastTouchEvent.y_velocity;
                io.MouseDown[0] = false;
            }
            else {
                io.MouseWheel = 0;
            }
            break;
        case TOUCH_ACTION_DOWN:
            io.MouseDown[0] = true;
            break;
        case TOUCH_ACTION_UP:
            io.MouseDown[0] = false;
            break;
        default:
            break;
    }
}

void ImGui_ImplAndroid_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    int w = g_SurfaceWidth, h = g_SurfaceHeight;
    int display_w = g_SurfaceWidth, display_h = g_SurfaceHeight;
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Android defaults to 3.0 scale
    io.FontGlobalScale = 3.0;

    // Setup time step
    float current_time = (float)get_time_us();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) / 1000000 : (float)(1.0f/60.0f);
    g_Time = current_time;

    ImGui_ImplAndroid_UpdateMousePosAndButtons();
}