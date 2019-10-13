#include "Editor/ProjectMenuWidget.hpp"

#include "Core/Application.hpp"
#include "Editor/Editor.hpp"
#include "Editor/ProjectFile.hpp"
#include "Editor/SelectFileDialog.hpp"
#include "Scene/Scene.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Saturn::Editor {

void ProjectMenuWidget::show(Application& app, Editor* editor, Scene& scene) {

    if (ImGui::Begin("Project Settings##WidgetWindow", get_shown_pointer())) {
        // left pane, categories
        show_category_pane();
        ImGui::SameLine();
        show_details_pane(app, editor, scene);
    }
    ImGui::End();
}

void ProjectMenuWidget::show_category_pane() {

    static auto selectable_flags = [this](SettingCategory this_cat) {
        return cat == this_cat;
    };

    ImGui::BeginChild("##ProjectSettingCat", ImVec2(200, 0), true);
    if (ImGui::Selectable("Info", selectable_flags(SettingCategory::Info))) {
        cat = SettingCategory::Info;
    }
    if (ImGui::Selectable("Render pipeline",
                          selectable_flags(SettingCategory::RenderPipeline))) {
        cat = SettingCategory::RenderPipeline;
    }
    if (ImGui::Selectable("Scenes",
                          selectable_flags(SettingCategory::Scenes))) {
        cat = SettingCategory::Scenes;
    }
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

void render_pipeline(RenderPipelineWidget& widget, Application& app) {
	ImGui::TextWrapped("%s", "Render Pipeline");
	ImGui::Separator();
    // ember render pipeline widget in project settings menu. Note that we
    // disabled the ImGui::Begin() and ImGui::End() calls in this widget to make
    // this possible
    widget.show(app);
}

static std::string get_scene_name_from_path(std::string const& path) {
    // Path looks a bit like
    // something/something/more/my_folder
    // We need my_folder extracted from that
    const std::size_t last_sep = path.find_last_of('\\');
    return path.substr(last_sep + 1);
}

void scene_details(fs::path const& scene) {
    ImGui::TextWrapped(
        "Path on disk: %s",
        fs::absolute(fs::relative(scene, ProjectFile::root_path()))
            .generic_string()
            .c_str());
    if (ProjectFile::main_scene() == scene) {
        ImGui::TextWrapped("This scene is the main scene");
    }
}

void scene_options(fs::path scene_path, Editor* editor, Scene& scene) {
    if (ImGui::Button("Load selected")) {
        editor->load_scene(scene, scene_path);
    }

    bool is_main =
        fs::absolute(scene_path) == fs::absolute(ProjectFile::main_scene());

    if (ImGui::Button("Set as main scene") && !is_main) {
        auto& scenes = ProjectFile::get_scene_paths();
        std::swap(scenes.front(), *std::find(scenes.begin(), scenes.end(),
                                             fs::absolute(scene_path)));
        ProjectFile::save();
    }

    // Delete button
    bool confirm_modal = false;
    ImGuiButtonFlags flags = ImGuiButtonFlags_None;
    // Disable deleting for main scene
    if (is_main) {
        flags |= ImGuiButtonFlags_Disabled;
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                            ImGui::GetStyle().Alpha * 0.5f);
    }

    if (ImGui::ButtonEx("Delete scene", ImVec2(0, 0), flags)) {
        confirm_modal = true;
    }
    if (flags & ImGuiButtonFlags_Disabled) { ImGui::PopStyleVar(); }
    if (confirm_modal) { ImGui::OpenPopup("Confirm delete?##SceneList"); }
    if (ImGui::BeginPopupModal("Confirm delete?##SceneList", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextWrapped(
            "Are you sure you want to delete scene %s ?",
            get_scene_name_from_path(scene_path.string()).c_str());
        if (ImGui::Button("Delete##DeleteScene")) {
            // Main scene cannot be deleted anyway, so we load this instead
            editor->load_scene(scene, ProjectFile::main_scene());
            // Do the actual deleting
            fs::remove_all(fs::absolute(scene_path));
            auto& paths = ProjectFile::get_scene_paths();
            paths.erase(std::find(paths.begin(), paths.end(),
                                  fs::absolute(scene_path)));
            ProjectFile::save();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##DeleteScene")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void scenes(Editor* editor, Scene& scene) {
    static fs::path selected = "";

    // Show list of scenes
    ImGui::BeginChild("##SceneListProjSettings", ImVec2(0, 150), false);
    ImGui::Text("Scene list");
    ImGui::Separator();
    for (auto& scene_p : ProjectFile::get_scene_paths()) {
        if (ImGui::Selectable(
                get_scene_name_from_path(scene_p.string()).c_str(),
                selected == scene_p)) {
            selected = scene_p;
        }
        if (ImGui::IsItemHovered()) {
            // Get absolute path and show it in a tooltip window
            ImGui::SetTooltip(
                "%s",
                fs::absolute(fs::relative(scene_p, ProjectFile::root_path()))
                    .generic_string()
                    .c_str());
        }
    }
    ImGui::EndChild();
    ImGui::BeginChild("##SceneDetailsProjSettings", ImVec2(0, 0), false);
    if (selected != "") { scene_details(selected); }
    ImGui::Separator();
    bool create_popup = false;
    if (ImGui::Button("Create scene")) { create_popup = true; }
    if (create_popup) { ImGui::OpenPopup("Create scene##ProjSettings"); }
    if (ImGui::BeginPopupModal("Create scene##ProjSettings", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
		constexpr size_t bufsize = 256;
        static std::string buf(bufsize, '\0');
        bool create = false;
        if (ImGui::InputText("Scene name", buf.data(), bufsize,
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
            create = true;
        }
        if (ImGui::Button("Create##NewSceneProjSettings") || create) {
            fs::path result = ProjectFile::root_path() / "scenes" /
                              buf.substr(0, buf.find_first_of('\0'));
            editor->create_new_scene(scene, result);
            editor->save_scene(scene);
            ProjectFile::get_scene_paths().push_back(result);
            ProjectFile::save();
            buf.clear();
            buf.resize(bufsize);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##NewSceneProjSettings")) {
            buf.clear();
            buf.resize(bufsize);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (selected != "") { scene_options(selected, editor, scene); }
    ImGui::EndChild();
}

} // namespace settings_pages

void ProjectMenuWidget::show_details_pane(Application& app,
                                          Editor* editor,
                                          Scene& scene) {
    // right pane, details
    ImGui::BeginChild("##ProjectSettingDetails", ImVec2(0, 0), true);
    switch (cat) {
        case SettingCategory::Info: settings_pages::info(); break;
        case SettingCategory::RenderPipeline:
            settings_pages::render_pipeline(pipeline_settings, app);
            break;
        case SettingCategory::Scenes:
            settings_pages::scenes(editor, scene);
            break;
        default: break;
    }
    ImGui::EndChild();
}

} // namespace Saturn::Editor
