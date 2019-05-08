#ifndef MVG_SCENE_HPP_
#define MVG_SCENE_HPP_

#include <memory>
#include <vector>

#include "SceneGraph.hpp"
#include "Transform.hpp"

#include "ECS.hpp"

#define COMPONENT_LIST ::Saturn::Components::Transform

namespace Saturn {

class SceneObject;

class Scene {
public:
    Scene();

    SceneGraph build_scene_graph();

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    ECS<COMPONENT_LIST> ecs;
};

} // namespace Saturn

#endif
