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

#    include <algorithm>
#    include <fmt/format.h>
#    include <fmt/ranges.h>
#    include <fstream>

namespace Saturn::Editor {

static std::string get_scene_name_from_path(std::string const& path) {
    // Path looks a bit like
    // something/something/more/my_folder
    // We need my_folder extracted from that
    const std::size_t last_sep = path.find_last_of('\\');
    return path.substr(last_sep + 1);
}

static std::string consistent_path_sep(std::string const& path) {
    auto copy = path;
    //    log::log(copy);
    std::replace(copy.begin(), copy.end(), '\\', '/');
    return copy;
}

static bool file_exists(std::string const& path) {
    std::ifstream file(path);
    return file.is_open();
}

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
    // Load last opened scene. We find this in
    // resources/engine_cache/last_scene.txt
    std::ifstream last_scene_file("resources/engine_cache/last_scene.txt");
    std::string last_scene;
    std::getline(last_scene_file, last_scene);
    cur_open_scene = get_scene_name_from_path(last_scene);
    cur_open_scene_full_path = last_scene;
    if (!file_exists(cur_open_scene_full_path + "/scene.dat")) {
        log::log(fmt::format("Last opened scene {} does not exist anymore. Has "
                             "it been deleted, renamed or moved?",
                             cur_open_scene_full_path),
                 DebugConsole::Warning);
        can_open_last = false;
    }
    set_window_title();
}

Editor::~Editor() {
    std::ofstream last_scene_cache("resources/engine_cache/last_scene.txt");
    last_scene_cache << cur_open_scene_full_path;
}

void Editor::set_window_title() {
    glfwSetWindowTitle(
        this->app->window(),
        ("SaturnEngine - " + consistent_path_sep(cur_open_scene_full_path))
            .c_str());
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
        // Load startup scene
        if (can_open_last) {
            load_scene(scene, cur_open_scene_full_path);
        } else {
            create_new_scene(scene, "resources/temp_scene");
        }

        // Setup some bindings
        ActionBinding exit_playmode_binding;
        exit_playmode_binding.key = Key::Escape;
        exit_playmode_binding.when = KeyAction::Release;
        exit_playmode_binding.callback = [this, &scene]() {
            if (playmode_active) {
                playmode_active = false;
                scene.deserialize_from_file(
                    "resources/playmode_temp/scene.dat");
                on_scene_reload(scene);
				// Disable mouse capture in editor.
                Input::set_mouse_capture(false);
            }
        };
        ActionBindingManager::add_action(exit_playmode_binding);
        once = false;
    }
    // #TODO: camera controls in editor

    // Enable mouse capture when space is pressed
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
    bool open_new_scene_popup = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New scene")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders);
                fs::path result = dialog.get_result();
                create_new_scene(scene, result);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save scene")) { save_scene(scene); }
            if (ImGui::MenuItem("Save as ...")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders);
                fs::path result = dialog.get_result();
                scene.serialize_to_file(result.string());
                log::log(fmt::format("Saved scene as {}",
                                     consistent_path_sep(result.string())));
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Open")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders);
                fs::path result = dialog.get_result();
                load_scene(scene, result);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { app->quit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::Selectable("Enter play mode")) {
                editor_widgets.entity_tree.reset_selected_entity();
                scene.serialize_to_file("resources/playmode_temp");
                on_playmode_enter(scene);
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

void Editor::on_scene_reload(Scene& scene) {
    log::log("Reloading scene");
    editor_widgets.entity_tree.reset_selected_entity();
    create_editor_camera(scene);
}

void Editor::create_entity(Scene& scene, std::string const& name) {
    using namespace Components;
    auto& obj = scene.create_object();
    auto& name_c = scene.get_ecs().get_with_id<Name>(obj.add_component<Name>());
    name_c.name = name;
    obj.add_component<Transform>();
    log::log(fmt::format("Creating entity with name: {}", name_c.name));
}

void Editor::load_scene(Scene& scene, fs::path path) {
    editor_widgets.entity_tree.reset_selected_entity();
    cur_open_scene = get_scene_name_from_path(path.string());
    cur_open_scene_full_path = path.string();
    log::log(fmt::format("Scene name is detected to be {}", cur_open_scene));
    path += "/scene.dat";
    scene.deserialize_from_file(path.string());
    on_scene_reload(scene);
    log::log(fmt::format("Loaded scene at path: {}",
                         consistent_path_sep(path.string())));
    set_window_title();
}

void Editor::save_scene(Scene& scene) {
    scene.serialize_to_file(cur_open_scene_full_path);
    log::log(fmt::format("Saved scene to {}",
                         consistent_path_sep(cur_open_scene_full_path)));
}

void Editor::create_new_scene(Scene& scene, fs::path path) {
    // 1. Load empty scene
    // 2. set paths and scene name to this scene's path
    // 3. save scene to new path

    load_scene(scene, "resources/empty_scene");
    cur_open_scene = get_scene_name_from_path(path.string());
    cur_open_scene_full_path = path.string();
    save_scene(scene);

    log::log(fmt::format("Created new scene at path {}",
                         consistent_path_sep(path.string())));
    set_window_title();
}

void Editor::create_editor_camera(Scene& scene) {
    log::log("Creating editor camera");
    using namespace Components;
    editor_camera = &scene.create_object();
    auto& cam_c = scene.get_ecs().get_with_id<Camera>(
        editor_camera->add_component<Camera>());
    auto& control_c = scene.get_ecs().get_with_id<EditorCameraController>(
        editor_camera->add_component<EditorCameraController>());
    editor_camera->add_component<DoNotSerialize>();
    auto& trans_c = scene.get_ecs().get_with_id<Transform>(
        editor_camera->add_component<Transform>());
    auto& name_c =
        scene.get_ecs().get_with_id<Name>(editor_camera->add_component<Name>());
    name_c.name = "EditorCamera";
    auto const& prefs = editor_widgets.preferences.get_preferences();
    control_c.sensitivity = prefs.camera.sensitivity;
    control_c.speed = prefs.camera.speed;
    control_c.zoom_speed = prefs.camera.zoom_speed;
    cam_c.viewport_id = scene_view_viewport_id;
    cam_c.fov = prefs.camera.fov;
    cam_c.front = glm::vec3(0.5, -0.5, 0.6);
    cam_c.up = glm::vec3(0, 1, 0);
    trans_c.position = glm::vec3(-10, 13, -11);
    trans_c.rotation = glm::vec3(-30, 52, 0);
    trans_c.scale = glm::vec3(1, 1, 1);
    app->get_renderer()
        ->get_viewport(scene_view_viewport_id)
        .set_camera(cam_c.id);
}

void Editor::on_playmode_enter(Scene& scene) {
    using namespace Components;
    Input::set_mouse_capture(true);
    for (auto [cam] : scene.get_ecs().select<Camera>()) {
        if (cam.entity == editor_camera)
            continue;
        else {
            auto vp = cam.viewport_id;
            app->get_renderer()->get_viewport(vp).set_camera(cam.id);
        }
    }
    scene.destroy_object(editor_camera);
    editor_camera = nullptr;
}

void Editor::frame_end() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Saturn::Editor

#endif
