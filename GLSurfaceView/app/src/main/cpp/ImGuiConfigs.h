//
// Created by plaZmadoZer on 6/15/19.
//

#ifndef IMGUI_ANDROID_IMGUICONFIGS_H
#define IMGUI_ANDROID_IMGUICONFIGS_H

#pragma once

#include <string>

typedef struct {
    float value;
    float min;
    float max;
} FLOAT_SLIDER;

class ImGuiConfigs {

public:
    bool checkBoxConfig = false;
    int intConfig = 5;
    FLOAT_SLIDER floatSlider = {.value = 0.3, .min = 0.0, .max = 1.0};

public:
    static ImGuiConfigs & GetInstance();

    std::string Serialize();
    static std::string Deserialize(ImGuiConfigs& out, std::string & content);

    void SaveToFile(std::string& configFile);
    void LoadFromFile(std::string& configFile);

private:
    ImGuiConfigs() {}
    ~ImGuiConfigs() {}
};

#endif //IMGUI_ANDROID_IMGUICONFIGS_H
