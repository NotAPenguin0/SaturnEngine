#ifdef WITH_EDITOR

#    include "Editor/Editor.hpp"
#    include "Core/Application.hpp"
#    include "Editor/SelectFileDialog.hpp"
#    include "Subsystems/ECS/ComponentList.hpp"
#    include "Subsystems/ECS/Components.hpp"
#    include "Subsystems/ECS/ECS.hpp"
#    include "Subsystems/Input/Input.hpp"
#    include "Subsystems/Renderer/Viewport.hpp"
#    include "Subsystems/Scene/Scene.hpp"
#    include "Subsystems/Serialization/ComponentMetaInfo.hpp"
#    include "imgui/imgui.h"
#    include "imgui/imgui_impl_glfw.h"
#    include "imgui/imgui_impl_opengl3.h"

#    include <fmt/format.h>
#    include <fmt/ranges.h>

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
    auto& console = log::get_console();
    console.add_entry("Initialization of Editor complete");
    console.add_command(
        "log", [&console](DebugConsole::CommandContext const& context) {
            console.add_entry(fmt::format("{}", join(context.args)));
        });
    log::log("The component editor does not support Resource<T> types yet",
             DebugConsole::Warning);
}

void Editor::setup_viewports() {
    log::log("Setting up Editor viewports");
    // Setup viewport for scene view
    auto main_cam = this->app->get_renderer()->get_viewport(0).get_camera();
    auto scene_view = Viewport(main_cam, 0, 0, 800, 600);
    scene_view_viewport_id =
        this->app->get_renderer()->add_viewport(scene_view);

    auto& main_view = this->app->get_renderer()->get_viewport(0);
    main_view.set_camera(-1);
}

SystemUpdateMode Editor::get_update_mode() {
    if (playmode_active) { return SystemUpdateMode::Play; }
    return SystemUpdateMode::Editor;
}

void Editor::render(Scene& scene) {
    static bool once = true;
    if (once) {
        ActionBinding exit_playmode_binding;
        exit_playmode_binding.key = Key::Escape;
        exit_playmode_binding.when = KeyAction::Release;
        exit_playmode_binding.callback = [this, &scene]() {
            if (playmode_active) {
                playmode_active = false;
                scene.deserialize_from_file(
                    "resources/playmode_temp/scene.dat");
                on_scene_reload();
            }
        };

        ActionBindingManager::add_action(exit_playmode_binding);
    }
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
    if (!playmode_active) {
        if (show_demo) { ImGui::ShowDemoWindow(&show_demo); }

        show_menu_bar(scene);

        if (log::get_console().is_shown()) { log::get_console().show(); }
        if (editor_widgets.entity_tree.is_shown()) {
            editor_widgets.entity_tree.show(scene);
        }
        if (editor_widgets.preferences.is_shown()) {
            editor_widgets.preferences.show();
        }
    }

    ImGui::Render();
    Viewport::set_active(
        app->get_renderer()->get_viewport(scene_view_viewport_id));
    glViewport(0, 0, 800, 600);
}

void Editor::show_menu_bar(Scene& scene) {
    using namespace Components;
    // Temporary
    static bool show_demo_window = false;

    bool open_new_entity_popup = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save scene to scene1")) {
                scene.serialize_to_file("resources/scene1");
                log::log("Saved scene to scene1");
            }
            if (ImGui::MenuItem("Open")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders);
                fs::path result = dialog.get_result();
                result += "/scene.dat";
                scene.deserialize_from_file(result.string());
                on_scene_reload();
                log::log(
                    fmt::format("Loaded scene at path: {}", result.string()));
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { app->quit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::Selectable("Enter play mode")) {
                scene.serialize_to_file("resources/playmode_temp");
                playmode_active = true;
            }
            if (ImGui::BeginMenu("Entity")) {
                if (ImGui::MenuItem("Create new entity")) {
                    open_new_entity_popup = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Entity Tree", nullptr,
                            editor_widgets.entity_tree.get_shown_pointer());
            ImGui::MenuItem("Debug Console", nullptr,
                            log::get_console().get_shown_pointer());
            ImGui::MenuItem("Editor Preferences", nullptr,
                            editor_widgets.preferences.get_shown_pointer());
            ImGui::MenuItem("ImGui Demo Window", nullptr, &show_demo_window);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Nothing here yet");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (open_new_entity_popup) { ImGui::OpenPopup("New..."); }
    if (ImGui::BeginPopupModal("New...", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
        static constexpr std::size_t bufsize = 256;
        static std::string entity_name_buffer(bufsize, '\0');
        if (ImGui::InputText("Entity Name", entity_name_buffer.data(), bufsize,
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
            create_entity(scene, entity_name_buffer);
            ImGui::CloseCurrentPopup();
            entity_name_buffer.clear();
            entity_name_buffer.resize(bufsize);
        }

        if (ImGui::Button("Create", ImVec2(120, 0))) {
            create_entity(scene, entity_name_buffer);
            ImGui::CloseCurrentPopup();
            entity_name_buffer.clear();
            entity_name_buffer.resize(bufsize);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            entity_name_buffer.clear();
            entity_name_buffer.resize(bufsize);
        }

        ImGui::EndPopup();
    }

    if (show_demo_window) ImGui::ShowDemoWindow();
}

void Editor::on_scene_reload() {
    log::log("Reloading scene");
    editor_widgets.entity_tree.reset_selected_entity();
}

void Editor::create_entity(Scene& scene, std::string const& name) {
    using namespace Components;
    auto& obj = scene.create_object();
    auto& name_c = scene.get_ecs().get_with_id<Name>(obj.add_component<Name>());
    name_c.name = name;
    obj.add_component<Transform>();
    log::log(fmt::format("Creating entity with name: {}", name_c.name));
}

void Editor::frame_end() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Saturn::Editor

#endif
