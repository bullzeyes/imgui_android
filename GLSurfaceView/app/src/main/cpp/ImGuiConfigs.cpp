//
// Created by plaZmadoZer on 6/15/19.
//

#include <stdio.h>
#include <picojson/picojson.h>
#include "ImGuiConfigs.h"

std::string ImGuiConfigs::Serialize()
{
    picojson::object obj;
    obj["intConfig"] = picojson::value((double)intConfig);
    obj["checkBoxConfig"] = picojson::value(checkBoxConfig);
    obj["floatSlider"] = picojson::value(picojson::object {
        {"value", picojson::value(floatSlider.value)},
        {"min", picojson::value(floatSlider.min)},
        {"max", picojson::value(floatSlider.max)}
    });

    return picojson::value(picojson::object{
            {"intConfig", picojson::value((double)intConfig)},
            {"checkBoxConfig", picojson::value(checkBoxConfig)},
            {"floatSlider", picojson::value(picojson::object {
                    {"value", picojson::value(floatSlider.value)},
                    {"min", picojson::value(floatSlider.min)},
                    {"max", picojson::value(floatSlider.max)}})
            }
    }).serialize();
}

std::string ImGuiConfigs::Deserialize(ImGuiConfigs& out, std::string & content)
{
    picojson::value value;
    std::string err = picojson::parse(value, content);
    if (err.empty()) {
        out.intConfig = value.get("intConfig").get<double>();
        out.checkBoxConfig = value.get("checkBoxConfig").get<bool>();
        out.floatSlider.value = value.get("floatSlider").get("value").get<double>();
        out.floatSlider.min = value.get("floatSlider").get("min").get<double>();
        out.floatSlider.max = value.get("floatSlider").get("max").get<double>();
    }

    return err;
}

ImGuiConfigs& ImGuiConfigs::GetInstance()
{
    static ImGuiConfigs instance;
    return instance;
}

void ImGuiConfigs::SaveToFile(std::string& configFile)
{
    std::string configContent = Serialize();
    FILE* fp = fopen(configFile.c_str(), "w");
    if (fp != NULL) {
        fwrite(configContent.c_str(), 1, configContent.length(), fp);
        fclose(fp);
    }
}

void ImGuiConfigs::LoadFromFile(std::string& configFile)
{
    FILE* fp = fopen(configFile.c_str(), "r");
    if (fp != NULL) {

        fseek (fp, 0, SEEK_END);
        long length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        std::vector<char> buffer(length + 1);
        fread(buffer.data(), 1, length, fp);
        buffer[buffer.size() - 1] = 0;
        fclose(fp);

        std::string configContent(buffer.data());
        ImGuiConfigs::Deserialize(*this, configContent);
    }
}