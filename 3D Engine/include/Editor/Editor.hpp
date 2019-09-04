#ifndef SATURN_EDITOR_HPP_
#define SATURN_EDITOR_HPP_

#ifdef WITH_EDITOR

#    include <cstddef>
#    include <vector>

#    include "Editor/DebugConsole.hpp"
#    include "Editor/EditorPreferencesWidget.hpp"
#    include "Editor/EntityTree.hpp"

namespace Saturn {

class Application;
class Scene;
class SceneObject;

namespace Editor {

class Editor {
public:
    Editor(Application& app);

    void setup_viewports();

    // Must be called at the start of the frame
    void render(Scene& scene);
    void frame_end();

    using EntityTreeT = std::vector<SceneObject*>;

private:
    void show_menu_bar(Scene& scene);
    void on_scene_reload();

    Application* app;
    std::size_t scene_view_viewport_id = 0;

    struct Widgets {
        DebugConsole debug_console;
        EntityTree entity_tree;
		EditorPreferencesWidget preferences;
    } editor_widgets;
};

} // namespace Editor
} // namespace Saturn

#endif
#endif
