#include "Editor/EditorPreferencesWidget.hpp"

#include "imgui/imgui.h"

#ifdef WITH_EDITOR

namespace Saturn::Editor {

void EditorPreferencesWidget::show() {
    if (ImGui::Begin("Editor preferences", &shown)) {
        ImGui::Text("Editor preferences");
    }
    ImGui::End();
}

} // namespace Saturn::Editor

#endif
