#ifndef MVG_SCENE_HPP_
#define MVG_SCENE_HPP_

#include <memory>
#include <vector>

#include "Components.hpp"
#include "SceneGraph.hpp"

#include "ECS.hpp"

#define COMPONENT_LIST                                                         \
    ::Saturn::Components::Transform, ::Saturn::Components::Camera

namespace Saturn {

class SceneObject;

class Scene {
public:
    friend class SceneObject;
    friend class ECS<COMPONENT_LIST>;
	friend class Renderer;

    Scene();
    ~Scene();

    SceneGraph build_scene_graph();

    SceneObject& create_object();

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    ECS<COMPONENT_LIST> ecs;
};

} // namespace Saturn

#endif
