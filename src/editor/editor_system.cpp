#include <editor/editor_system.hpp>
#include <phobos/present/present_manager.hpp>

#include <imgui/imgui.h>

void EditorSystem::update(saturn::FrameContext& ctx) {
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                         ImGuiDockNodeFlags_None);
    }

    static bool show_scene = true;
    if (ImGui::Begin("Scene", &show_scene)) {
        auto img = ctx.render_info.present_manager->get_attachment(ctx.render_info, "color1");
        ImGui::Image(img.get_imgui_tex_id(), ImVec2(img.get_width(), img.get_height()));
    }

    ImGui::End();
}