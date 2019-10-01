#include "Editor/EditorPreferencesWidget.hpp"

#include "Editor/EditorLog.hpp"

#include "imgui/imgui.h"
#include <nlohmann/json.hpp>

#include <fstream>
#include <string>

#ifdef WITH_EDITOR

namespace Saturn::Editor {

void EditorPreferencesWidget::load(fs::path const& path) {
    std::ifstream data(path);
    if (!data.good()) {
        log::warn(
            "Failed to load editor preferences at path {}. Will use default "
            "values instead",
            path.string());
        return;
    }

    nlohmann::json j;
    data >> j;

    prefs.camera.sensitivity = j["Preferences"]["Camera"]["Sensitivity"];
    prefs.camera.speed = j["Preferences"]["Camera"]["Speed"];
    prefs.camera.zoom_speed = j["Preferences"]["Camera"]["ZoomSpeed"];
    prefs.camera.fov = j["Preferences"]["Camera"]["FOV"];
    prefs.camera.invert_x = j["Preferences"]["Camera"]["InvertX"];
    prefs.camera.invert_y = j["Preferences"]["Camera"]["InvertY"];
}

void EditorPreferencesWidget::save(fs::path const& path) {
    nlohmann::json j;
    j["Preferences"] = nlohmann::json::object();
    j["Preferences"]["Camera"] = nlohmann::json::object();
    j["Preferences"]["Camera"]["Sensitivity"] = prefs.camera.sensitivity;
    j["Preferences"]["Camera"]["Speed"] = prefs.camera.speed;
    j["Preferences"]["Camera"]["ZoomSpeed"] = prefs.camera.zoom_speed;
    j["Preferences"]["Camera"]["FOV"] = prefs.camera.fov;
    j["Preferences"]["Camera"]["InvertY"] = prefs.camera.invert_x;
    j["Preferences"]["Camera"]["InvertX"] = prefs.camera.invert_y;

    std::ofstream data(path);
    data << j.dump(4);
}

void EditorPreferencesWidget::show() {
    if (ImGui::Begin("Editor preferences", &shown)) {
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::Text("These values will be updated the next time you exit "
                        "play mode");
            ImGui::DragScalar("Sensitivity##Prefs", ImGuiDataType_Float,
                              &prefs.camera.sensitivity, 0.02f);
            ImGui::DragScalar("Speed##Prefs", ImGuiDataType_Float,
                              &prefs.camera.speed, 0.02f);
            ImGui::DragScalar("Zoom Speed##Prefs", ImGuiDataType_Float,
                              &prefs.camera.zoom_speed, 0.2f);
            ImGui::DragScalar("FOV##Prefs", ImGuiDataType_Float,
                              &prefs.camera.fov, 0.2f);
            ImGui::Checkbox("Invert X##Prefs", &prefs.camera.invert_x);
            ImGui::Checkbox("Invert Y##Prefs", &prefs.camera.invert_y);
        }
        if (ImGui::CollapsingHeader("Style##Prefs")) {
            static int style_index = -1;
            if (ImGui::Combo("Colors: ##Prefs", &style_index,
                             "Classic\0Dark\0Light\0")) {
                switch (style_index) {
                    case 0: ImGui::StyleColorsClassic(); break;
                    case 1: ImGui::StyleColorsDark(); break;
                    case 2: ImGui::StyleColorsLight(); break;
                    default: ImGui::StyleColorsDark();
                }
            }
        }
    }
    ImGui::End();
}

EditorPreferencesWidget::Preferences const&
EditorPreferencesWidget::get_preferences() const {
    return prefs;
}

} // namespace Saturn::Editor

#endif
