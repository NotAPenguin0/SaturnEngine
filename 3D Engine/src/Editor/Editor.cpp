#ifdef WITH_EDITOR

#    include "Editor/Editor.hpp"
#    include "Core/Application.hpp"
#    include "Editor/SelectFileDialog.hpp"
#    include "Subsystems/ECS/ComponentList.hpp"
#    include "Subsystems/ECS/Components.hpp"
#    include "Subsystems/Input/Input.hpp"
#    include "Subsystems/Renderer/Viewport.hpp"
#    include "Subsystems/Scene/Scene.hpp"
#    include "Subsystems/Serialization/ComponentMetaInfo.hpp"
#    include "imgui/imgui.h"
#    include "imgui/imgui_impl_glfw.h"
#    include "imgui/imgui_impl_opengl3.h"

#    include <fmt/format.h>

namespace Saturn::Editor {

Editor::Editor(Application& app) : app(&app) {
    this->app->set_editor_instance(this);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->app->window_handle, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    // Initialize components metadata
    Meta::ComponentsMeta<COMPONENT_LIST>::init();
    editor_widgets.debug_console.add_entry("Initialization of Editor complete");
}

void Editor::setup_viewports() {
    editor_widgets.debug_console.add_entry("Setting up Editor viewports");
    // Setup viewport for scene view
    auto main_cam = this->app->get_renderer()->get_viewport(0).get_camera();
    auto scene_view = Viewport(main_cam, 0, 0, 800, 600);
    scene_view_viewport_id =
        this->app->get_renderer()->add_viewport(scene_view);

    auto& main_view = this->app->get_renderer()->get_viewport(0);
    main_view.set_camera(-1);
}

void Editor::render(Scene& scene) {
    // #TODO: camera controls in editor
    // #TODO: specify which ECS systems run in editor and which don't

    // Enable mouse capture when space is pressed
    Input::set_mouse_capture(RawInput::get_key(Key::Space).down);
    ImGui::GetIO().WantCaptureMouse = false;

    static bool show_demo = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, app->window_dimensions.x, app->window_dimensions.y);
    glClear(GL_COLOR_BUFFER_BIT);
    if (show_demo) { ImGui::ShowDemoWindow(&show_demo); }

    show_menu_bar(scene);

    if (editor_widgets.debug_console.is_shown()) {
        editor_widgets.debug_console.show();
    }
    if (editor_widgets.entity_tree.is_shown()) {
        editor_widgets.entity_tree.show(scene);
    }

    ImGui::Render();
    Viewport::set_active(
        app->get_renderer()->get_viewport(scene_view_viewport_id));
    glViewport(0, 0, 800, 600);
}

void Editor::show_menu_bar(Scene& scene) {
    // Temporary
    static bool show_demo_window = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save scene to scene1")) {
                scene.serialize_to_file("resources/scene1");
            }
            if (ImGui::MenuItem("Open")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders);
                fs::path result = dialog.get_result();
                result += "/scene.dat";
                scene.deserialize_from_file(result.string());
                on_scene_reload();
                editor_widgets.debug_console.add_entry(
                    fmt::format("Loaded scene at path: {}", result.string()));
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { app->quit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Nothing here yet");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Entity Tree", nullptr,
                            editor_widgets.entity_tree.get_shown_pointer());
            ImGui::MenuItem("Debug Console", nullptr,
                            editor_widgets.debug_console.get_shown_pointer());
            ImGui::MenuItem("ImGui Demo Window", nullptr, &show_demo_window);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Nothing here yet");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (show_demo_window) ImGui::ShowDemoWindow();
}

void Editor::on_scene_reload() {
    editor_widgets.debug_console.add_entry("Reloading scene");
    editor_widgets.entity_tree.reset_selected_entity();
}

void Editor::frame_end() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Saturn::Editor

#endif
