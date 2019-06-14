//
// Created by Xiaoxing Zhu on 6/13/19.
//

#include <imgui/imgui.h>
#include <bits/timespec.h>
#include <time.h>
#include "imgui/imgui.h"
#include "imgui_impl_android.h"

static double g_Time;

long get_time_us()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000 + (now.tv_nsec / 1000);
}

bool ImGui_ImplAndroid_InitForOpenGL()
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "imgui_impl_android";
    return true;
}

bool ImGui_ImplAndroid_InitForVulkan()
{
    return true;
}

void ImGui_ImplAndroid_Shutdown()
{

}

void ImGui_ImplAndroid_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    int w = 1080, h = 1920;
    int display_w = 1080, display_h = 1920;
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step
    double current_time = (double)get_time_us();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
    g_Time = current_time;
}