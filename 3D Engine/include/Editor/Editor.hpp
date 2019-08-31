#ifndef SATURN_EDITOR_HPP_
#define SATURN_EDITOR_HPP_

#ifdef WITH_EDITOR

#include <cstddef>
#include <vector>

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
	void show_scene_tree(Scene& scene);
	void show_entity_tree(EntityTreeT& enttree, Scene& scene, SceneObject*& selected);
	void show_entity_details(SceneObject* entity, Scene& scene);
    void show_menu_bar(Scene& scene);

    Application* app;
    std::size_t scene_view_viewport_id = 0;

	struct WidgetsShowData {
		bool entity_tree = true;
	} show_widgets;

	std::vector<SceneObject*> build_entity_tree(Scene& scene);
};

} // namespace Editor
} // namespace Saturn

#endif
#endif