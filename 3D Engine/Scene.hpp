#ifndef MVG_SCENE_HPP_
#define MVG_SCENE_HPP_

#include <memory>
#include <vector>

#include "SceneGraph.hpp"

#include "component_container.hpp"
#include "component_index.hpp"

#define COMPONENT_LIST /*All components must be listed here, comma separated*/

namespace Saturn {

class SceneObject;

class Scene {
public:
    Scene();

    SceneGraph build_scene_graph();

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    std::vector<any_component_container> components;

    component_index_table<COMPONENT_LIST> component_indices;
};

} // namespace Saturn

#endif
