#ifdef WITH_EDITOR

#    include "Editor/Editor.hpp"
#    include "Core/Application.hpp"
#    include "imgui/imgui.h"
#    include "imgui/imgui_impl_glfw.h"
#    include "imgui/imgui_impl_opengl3.h"

namespace Saturn::Editor {

Editor::Editor(Application& app) : app(&app) {
    this->app->set_editor_instance(this);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->app->window_handle, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    
}

void Editor::setup_viewports() {
	// Setup viewport for scene view
	auto main_cam = this->app->get_renderer()->get_viewport(0).get_camera();
	auto scene_view = Viewport(main_cam, 0, 0, 800, 600);
    scene_view_viewport_id = this->app->get_renderer()->add_viewport(scene_view);

	auto& main_view = this->app->get_renderer()->get_viewport(0);
	main_view.set_camera(-1);
}

void Editor::render() {
    static bool show_demo = true;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
//    Viewport::set_active(app->renderer->get_viewport(main_vp_id));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, app->window_dimensions.x, app->window_dimensions.y);
    glClear(GL_COLOR_BUFFER_BIT);
    if (show_demo) { ImGui::ShowDemoWindow(&show_demo); }

    ImGui::Render();
    Viewport::set_active(
        app->get_renderer()->get_viewport(scene_view_viewport_id));
    glViewport(0, 0, 800, 600);
}

void Editor::frame_end() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Saturn::Editor

#endif
