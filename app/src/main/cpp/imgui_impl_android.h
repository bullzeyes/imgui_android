//
// Created by Xiaoxing Zhu on 6/13/19.
//

#ifndef IMGUI_ANDROID_IMGUI_IMPL_ANDROID_H
#define IMGUI_ANDROID_IMGUI_IMPL_ANDROID_H

#pragma once

IMGUI_IMPL_API bool     ImGui_ImplAndroid_InitForOpenGL(int surfaceWidth, int surfaceHeight, int windowWidth, int windowHeight, const char* outputDir);
IMGUI_IMPL_API bool     ImGui_ImplAndroid_InitForVulkan(int surfaceWidth, int surfaceHeight, int windowWidth, int windowHeight, const char* outputDir);
IMGUI_IMPL_API void     ImGui_ImplAndroid_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplAndroid_NewFrame();

IMGUI_IMPL_API void     ImGui_ImplAndroid_UpdateTouchEvent(int a, float x, float y, int pointers);

typedef enum {
    TOUCH_ACTION_DOWN = 0,
    TOUCH_ACTION_UP,
    TOUCH_ACTION_MOVE,
    TOUCH_ACTION_CANCEL,
    TOUCH_ACTION_OUTSIDE,
    TOUCH_ACTION_POINTER_DOWN,
    TOUCH_ACTION_POINTER_UP,
    TOUCH_ACTION_HOVER_MOVE,
    TOUCH_ACTION_SCROLL,
    TOUCH_ACTION_HOVER_ENTER,
    TOUCH_ACTION_HOVER_EXIT,
    TOUCH_ACTION_BUTTON_PRESS,
    TOUCH_ACTION_BUTTON_RELEASE,
    TOUCH_ACTION_COUNT
} TOUCH_ACTION;

typedef struct {
    TOUCH_ACTION action;
    float x;
    float y;
    int pointers;
    float y_velocity;
    float x_velocity;
}TOUCH_EVENT;

#endif //IMGUI_ANDROID_IMGUI_IMPL_ANDROID_H
