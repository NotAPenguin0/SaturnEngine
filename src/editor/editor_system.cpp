#include <editor/editor_system.hpp>
#include <phobos/present/present_manager.hpp>

#include <imgui/imgui.h>

void EditorSystem::update(saturn::FrameContext& ctx) {
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainEditorSpace", nullptr, flags);
    ImGui::PopStyleVar(3);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                         ImGuiDockNodeFlags_None);
    }

    static bool show_scene = true;
    if (ImGui::Begin("Scene", &show_scene, ImGuiWindowFlags_HorizontalScrollbar)) {
        auto& img = ctx.render_info.present_manager->get_attachment("color1");
        auto& depth = ctx.render_info.present_manager->get_attachment("depth1");
        auto const size = ImGui::GetWindowSize();
        img.resize(size.x, size.y);
        depth.resize(size.x, size.y);
        ImGui::Image(img.get_imgui_tex_id(), ImVec2(img.get_width(), img.get_height()));
    }

    ImGui::End();

    // End dockspace window
    ImGui::End();
}