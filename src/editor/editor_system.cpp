#include <editor/editor_system.hpp>
#include <phobos/present/present_manager.hpp>

#include <phobos/util/cmdbuf_util.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>

EditorSystem::EditorSystem(LogWindow* log_window) : log_window(log_window) {

}

void EditorSystem::startup(ph::VulkanContext& ctx) {
    ImGuiIO io = ImGui::GetIO();
    editor_font = io.Fonts->AddFontFromFileTTF("data/fonts/heebo/Heebo-Regular.ttf", 16.0f);

    // Upload font textures
    vk::CommandBuffer cmd_buf = ph::begin_single_time_command_buffer(ctx);
    
    ImGui_ImplVulkan_CreateFontsTexture(cmd_buf);

    ph::end_single_time_command_buffer(ctx, cmd_buf);
    ctx.device.waitIdle();
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

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
             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainEditorSpace", nullptr, flags);
    ImGui::PopStyleVar(3);

    ImGui::PushFont(editor_font);

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
        auto const size = ImGui::GetContentRegionAvail();
        img.resize(size.x, size.y);
        depth.resize(size.x, size.y);
        ImGui::Image(img.get_imgui_tex_id(), ImVec2(img.get_width(), img.get_height()));
    }

    ImGui::End();

    log_window->show_gui();

    ImGui::PopFont();

    // End dockspace window
    ImGui::End();
}