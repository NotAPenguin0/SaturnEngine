#include "Editor/EditorPreferencesWidget.hpp"

#include "imgui/imgui.h"

#ifdef WITH_EDITOR

namespace Saturn::Editor {

void EditorPreferencesWidget::show() {
    if (ImGui::Begin("Editor preferences", &shown)) {
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::Text("These values will be updated the next time you exit play mode");
			ImGui::DragScalar("Sensitivity", ImGuiDataType_Float, &prefs.camera.sensitivity, 0.02f);
			ImGui::DragScalar("Speed", ImGuiDataType_Float, &prefs.camera.speed, 0.02f);
            ImGui::DragScalar("Zoom Speed", ImGuiDataType_Float,
                              &prefs.camera.zoom_speed, 0.2f);
            ImGui::DragScalar("FOV", ImGuiDataType_Float, &prefs.camera.fov,
                              0.2f);
		}
        if (ImGui::CollapsingHeader("Style")) {
            static int style_index = -1;
            if (ImGui::Combo("Colors: ", &style_index,
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
