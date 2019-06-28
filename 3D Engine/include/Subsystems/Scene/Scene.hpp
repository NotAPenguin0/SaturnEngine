#ifndef MVG_SCENE_HPP_
#define MVG_SCENE_HPP_

#include <memory>
#include <vector>
#include <string_view>

#include "Subsystems/ECS/Components.hpp"

#include "Subsystems/ECS/ECS.hpp"
#include "Subsystems/ECS/ComponentList.hpp"


namespace Saturn {

class Application;

class SceneObject;

class Scene {
public:
    friend class SceneObject;
    friend class ECS<COMPONENT_LIST>;
    friend class Renderer;
    friend class ::Saturn::Application;
    friend class Systems::SystemBase;

    Scene(Application* app);
    ~Scene();

	void on_start();
	void update_systems();

    SceneObject& create_object(SceneObject* parent = nullptr);
    SceneObject& create_object_from_file(std::string_view file_path, SceneObject* parent = nullptr);

	ECS<COMPONENT_LIST>& get_ecs();

	void serialize_to_file(std::string_view folder);
	void deserialize_from_file(std::string_view path);

	Application* get_app();

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    ECS<COMPONENT_LIST> ecs;
	Application* app;
};

} // namespace Saturn

#endif
