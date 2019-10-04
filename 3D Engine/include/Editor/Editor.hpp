#ifndef SATURN_EDITOR_HPP_
#define SATURN_EDITOR_HPP_

#ifdef WITH_EDITOR

#    include <cstddef>
#    include <filesystem>
#    include <vector>

#    include "Editor/EditorLog.hpp"
#    include "Editor/EditorPreferencesWidget.hpp"
#    include "Editor/EntityTree.hpp"
#    include "Editor/FPSOverlay.hpp"
#    include "ProjectMenuWidget.hpp"

namespace fs = std::filesystem;

namespace Saturn {

enum class SystemUpdateMode;

class Application;
class Scene;
class SceneObject;

namespace Editor {

class Editor {
public:
    Editor(Application& app);
    ~Editor();

    void setup_viewports();
    SystemUpdateMode get_update_mode();

    // Must be called at the start of the frame
    void render(Scene& scene);
    void frame_end();

	void create_new_scene(Scene& scene, fs::path path);
    void load_scene(Scene& scene, fs::path path, bool use_project_dir = true);
    void save_scene(Scene& scene);

private:
    void show_menu_bar(Scene& scene);
    void on_scene_reload(Scene& scene);
    void create_entity(Scene& scene, std::string const& name);
    void create_editor_camera(Scene& scene);
    void on_playmode_enter(Scene& scene);
    void set_window_title();
   

    Application* app;
    SceneObject* editor_camera = nullptr;
    std::size_t scene_view_viewport_id = 0;

    std::string cur_open_scene;
    std::string cur_open_scene_full_path;
    bool can_open_last = true;

    struct Widgets {
        EntityTree entity_tree;
        EditorPreferencesWidget preferences;
        FPSOverlay fps_overlay;
        ProjectMenuWidget project_menu;
    } editor_widgets;

    bool playmode_active = false;
};

} // namespace Editor
} // namespace Saturn

#endif
#endif
