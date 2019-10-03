#ifndef SATURN_SCENE_HPP_
#define SATURN_SCENE_HPP_

#include <memory>
#include <string_view>
#include <vector>

#include "Subsystems/ECS/Components.hpp"

#include "Subsystems/ECS/ComponentList.hpp"
#include "Subsystems/ECS/ECS.hpp"

namespace Saturn {

class Application;

class SceneObject;

#ifdef WITH_EDITOR
namespace Editor {
class Editor;
class EntityTree;
}
#endif

class Scene {
public:
    friend class SceneObject;
    friend class ECS<COMPONENT_LIST>;
    friend class Renderer;
    friend class ::Saturn::Application;
    friend class Systems::SystemBase;
#ifdef WITH_EDITOR
    friend class ::Saturn::Editor::Editor;
	friend class ::Saturn::Editor::EntityTree;
#endif

    Scene(Application* app);
    ~Scene();

    void on_start();
	void on_exit();
    void update_systems(SystemUpdateMode mode);

    SceneObject& create_object(SceneObject* parent = nullptr);
    SceneObject& create_object_from_file(std::string_view file_path,
                                         SceneObject* parent = nullptr);
	void destroy_object(SceneObject* object);

	SceneObject& get_entity_by_name(std::string_view name);

    ECS<COMPONENT_LIST>& get_ecs();

    void serialize_to_file(std::string_view folder);
    void deserialize_from_file(std::string_view path, bool use_project_dir = true);
	void clear_scene();

    Application* get_app();

private:
    void resolve_parent_pointers();
    void set_id_generator_value();
    std::vector<std::unique_ptr<SceneObject>> objects;
    ECS<COMPONENT_LIST> ecs;
    Application* app;
};

} // namespace Saturn

#endif
