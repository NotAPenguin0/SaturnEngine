#include "Editor/ProjectMenuWidget.hpp"

#include "Editor/ProjectFile.hpp"

#include "imgui/imgui.h"

namespace Saturn::Editor {

void ProjectMenuWidget::show() {

    if (ImGui::Begin("Project Settings##WidgetWindow", get_shown_pointer())) {
        // left pane, categories
        show_category_pane();
        ImGui::SameLine();
        show_details_pane();
    }
    ImGui::End();
}

void ProjectMenuWidget::show_category_pane() {
    ImGui::BeginChild("##ProjectSettingCat", ImVec2(200, 0), true);
    if (ImGui::Selectable("Info")) { cat = SettingCategory::Info; }
    if (ImGui::Selectable("Render pipeline")) {
        cat = SettingCategory::RenderPipeline;
    }
    if (ImGui::Selectable("Scenes")) { cat = SettingCategory::Scenes; }
    ImGui::EndChild();
}

namespace settings_pages {

void info() {
    ImGui::Text("Project info");
    ImGui::Separator();
    ImGui::TextWrapped("Name: %s",
                       ProjectFile::name().generic_string().c_str());
    ImGui::TextWrapped("Location on disk: %s",
                       ProjectFile::root_path().generic_string().c_str());
}

void render_pipeline() {}

void scenes() {}

} // namespace settings_pages

void ProjectMenuWidget::show_details_pane() {
    // right pane, details
    ImGui::BeginChild("##ProjectSettingDetails", ImVec2(0, 0), true);
    switch (cat) {
        case SettingCategory::Info: settings_pages::info(); break;
        case SettingCategory::RenderPipeline: settings_pages::render_pipeline();
        case SettingCategory::Scenes: settings_pages::scenes(); break;
        default: break;
    }
    ImGui::EndChild();
}

} // namespace Saturn::Editor
