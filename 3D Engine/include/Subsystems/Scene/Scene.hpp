#ifndef MVG_SCENE_HPP_
#define MVG_SCENE_HPP_

#include <memory>
#include <vector>

#include "Subsystems/ECS/Components.hpp"

#include "Subsystems/ECS/ECS.hpp"
#include "Subsystems/ECS/ComponentList.hpp"


namespace Saturn {

class SceneObject;

class Scene {
public:
    friend class SceneObject;
    friend class ECS<COMPONENT_LIST>;
    friend class Renderer;
    friend class Application;
    friend class Systems::SystemBase;

    Scene();
    ~Scene();

	void on_start();
	void update_systems();

    SceneObject& create_object(SceneObject* parent = nullptr);

	ECS<COMPONENT_LIST>& get_ecs();

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    ECS<COMPONENT_LIST> ecs;
};

} // namespace Saturn

#endif
