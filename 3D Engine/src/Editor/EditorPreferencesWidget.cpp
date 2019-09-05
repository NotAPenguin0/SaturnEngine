#include "Editor/EditorPreferencesWidget.hpp"

#include "imgui/imgui.h"

#ifdef WITH_EDITOR

namespace Saturn::Editor {

void EditorPreferencesWidget::show() {
    if (ImGui::Begin("Editor preferences", &shown)) {
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

} // namespace Saturn::Editor

#endif
