#ifndef MVG_SCENE_HPP_
#define MVG_SCENE_HPP_

#include <memory>
#include <vector>

#include "Components.hpp"
#include "SceneGraph.hpp"

#include "ECS.hpp"
#include "ComponentList.hpp"


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

	void update_systems();
    SceneGraph build_scene_graph();

    SceneObject& create_object();

	ECS<COMPONENT_LIST>& get_ecs();

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    ECS<COMPONENT_LIST> ecs;
};

} // namespace Saturn

#endif
