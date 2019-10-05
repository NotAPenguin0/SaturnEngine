#ifdef WITH_EDITOR
#    include "Editor/Editor.hpp"

#    include "Core/Application.hpp"
#    include "Editor/ProjectFile.hpp"
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
#    include <fmt/ostream.h>
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

template<typename R>
static void log_resources(bool show_editor) {
    for (auto const& [id, asset] : AssetManager<R>::resource_list()) {
        if (asset.imported || show_editor) {
            log::log("resource with id {} loaded from path {}", id,
                     asset.path.generic_string());
        }
    }
}

static void do_imports() {
    // Import required assets
    AssetManager<Shader>::do_imports();
    AssetManager<Texture>::do_imports();
    AssetManager<Mesh>::do_imports();
    AssetManager<audeo::SoundSource>::do_imports();
}

Editor::Editor(Application& app) : app(&app) {
    this->app->set_editor_instance(this);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = false;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
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

    console.add_command(
        "clear", [&console](DebugConsole::CommandContext const& context) {
            console.clear();
        });

    console.add_command("assets", [](DebugConsole::CommandContext context) {
        if (context.args.empty() || context.args.size() > 2) {
            log::error("Invalid argument count for command 'assets'. "
                       "Expected 1 or 2, found {}",
                       context.args.size());
            return;
        }

        bool show_non_imported = false;

        if (context.args.size() >= 2) {
            auto& import_mod = context.args[1];
            import_mod = import_mod.substr(0, import_mod.find_first_of('\0'));
            if (import_mod == "imported") {
                show_non_imported = false;
            } else if (import_mod == "editor") {
                show_non_imported = true;
            } else {
                log::error(
                    "Invalid argument (2) for command 'assets'. Expected "
                    "either 'imported' or 'editor', found {}",
                    import_mod);
            }
        }

        auto& type = context.args[0];
        type = type.substr(0, type.find_first_of('\0'));
        using namespace std::literals::string_literals;
        if (type == "shader"s) {
            log_resources<Shader>(show_non_imported);
            return;
        }
        if (type == "texture"s) {
            log_resources<Texture>(show_non_imported);
            return;
        }
        if (type == "mesh"s) {
            log_resources<Mesh>(show_non_imported);
            return;
        }
        if (type == "sound"s) {
            log_resources<audeo::SoundSource>(show_non_imported);
            return;
        }
    });

    // Load preferences
    log::log("Loading preferences");
    editor_widgets.preferences.load("config/settings.json");

    // Load last opened project
    std::ifstream last_project_file("config/last_project.ini");
    std::string last_project;
    std::getline(last_project_file, last_project);
    ProjectFile::load(last_project);
    cur_open_scene =
        get_scene_name_from_path(ProjectFile::main_scene().string());
    cur_open_scene_full_path =
        fs::absolute(ProjectFile::main_scene()).generic_string();
    if (!file_exists(cur_open_scene_full_path + "/scene.dat")) {
        log::warn("Last opened scene {} does not exist anymore. Has "
                  "it been deleted, renamed or moved?",
                  cur_open_scene_full_path);
        can_open_last = false;
    }

    // Load render stages
    for (auto const& stage_data : ProjectFile::get_render_stages()) {
        editor_widgets.project_menu.pipeline_settings.add_stage(
            app, stage_data.type, stage_data.stage);
    }

    do_imports();

    set_window_title();
} // namespace Saturn::Editor

Editor::~Editor() {
    std::ofstream last_project_cache("config/last_project.ini");
    last_project_cache << ProjectFile::path();
    editor_widgets.preferences.save("resources/engine_cache/settings.json");
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
                scene.on_exit();
                scene.deserialize_from_file("config/temp/playmode/scene.dat",
                                            false);
                on_scene_reload(scene);
                // Disable mouse capture in editor.
                Input::set_mouse_capture(false);
            }
        };
        ActionBindingManager::add_action(exit_playmode_binding);
        once = false;
    }

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
        if (editor_widgets.fps_overlay.is_shown()) {
            editor_widgets.fps_overlay.show();
        }
        if (editor_widgets.project_menu.is_shown()) {
            editor_widgets.project_menu.show(*app, this, scene);
        }
        if (editor_widgets.asset_browser.is_shown()) {
            editor_widgets.asset_browser.show();
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
                dialog.show(SelectFileDialog::PickFolders,
                            fs::absolute(fs::path("resources/")));

                fs::path result = dialog.get_result();
                if (result != "") { create_new_scene(scene, result); }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save scene")) { save_scene(scene); }
            if (ImGui::MenuItem("Save scene as ...")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders,
                            fs::absolute(fs::path("resources/")));
                fs::path result = dialog.get_result();
                if (result != "") {
                    scene.serialize_to_file(result.string());
                    log::log("Saved scene as {}", result.generic_string());
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Open scene")) {
                static SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFolders,
                            fs::absolute(ProjectFile::root_path()));
                fs::path result = dialog.get_result();
                if (result != "") { load_scene(scene, result); }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("New project")) {
                static SelectFileDialog dialog;
                dialog.show(
                    SelectFileDialog::PickFolders,
                    fs::absolute(ProjectFile::root_path()).parent_path());
                fs::path result = dialog.get_result();
                if (result != "") {
                    ProjectFile::create_and_load(result);
                    create_new_scene(scene, ProjectFile::main_scene());
                }
            }

            if (ImGui::MenuItem("Open project")) {
                static SelectFileDialog dialog;
                dialog.show(
                    SelectFileDialog::PickFiles,
                    fs::absolute(ProjectFile::root_path()).parent_path(),
                    {FileType{L"Saturn Engine Project (*.seproj)",
                              L"*.seproj"}});
                fs::path result = dialog.get_result();
                if (result != "") {
                    ProjectFile::load(result);
                    load_scene(scene, ProjectFile::main_scene());
                }
            }

            if (ImGui::MenuItem("Save project")) {
                save_scene(scene);
                ProjectFile::save();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { app->quit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::Selectable("Enter play mode")) {
                editor_widgets.entity_tree.reset_selected_entity();
                scene.serialize_to_file("config/temp/playmode");
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
            ImGui::MenuItem("Project settings", nullptr,
                            editor_widgets.project_menu.get_shown_pointer());
            ImGui::MenuItem("Editor Preferences", nullptr,
                            editor_widgets.preferences.get_shown_pointer());
            ImGui::MenuItem("FPS overlay", nullptr,
                            editor_widgets.fps_overlay.get_shown_pointer());
            ImGui::MenuItem("Asset Browser", nullptr,
                            editor_widgets.asset_browser.get_shown_pointer());
            ImGui::MenuItem("ImGui Demo Window", nullptr, &show_demo_window);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) { ImGui::EndMenu(); }

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
    log::log("Creating entity with name: {}", name_c.name);
}

void Editor::load_scene(Scene& scene, fs::path path, bool use_project_dir) {

    do_imports();

    editor_widgets.entity_tree.reset_selected_entity();
    cur_open_scene = get_scene_name_from_path(path.string());
    cur_open_scene_full_path = path.string();
    log::log("Scene name is detected to be {}", cur_open_scene);
    path += "/scene.dat";
    scene.deserialize_from_file(path.string(), use_project_dir);
    on_scene_reload(scene);
    log::log("Loaded scene at path: {}", path.generic_string());
    set_window_title();
}

void Editor::save_scene(Scene& scene) {
    scene.serialize_to_file(cur_open_scene_full_path);
    log::log("Saved scene to {}",
             consistent_path_sep(cur_open_scene_full_path));
}

void Editor::create_new_scene(Scene& scene, fs::path path) {
    // 1. Load empty scene
    // 2. set paths and scene name to this scene's path
    // 3. save scene to new path

    load_scene(scene, fs::absolute("resources/empty_scene").generic_string(),
               false);
    cur_open_scene = get_scene_name_from_path(path.string());
    cur_open_scene_full_path = path.string();
    save_scene(scene);

    log::log("Created new scene at path {}", path.generic_string());
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
    control_c.invert_x = prefs.camera.invert_x;
    control_c.invert_y = prefs.camera.invert_y;
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
    scene.on_start();
}

void Editor::frame_end() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    auto& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace Saturn::Editor

#endif
