#ifndef MVG_SCENE_GRAPH_HPP_
#define MVG_SCENE_GRAPH_HPP_

#include "Shader.hpp"
#include "VertexArray.hpp"

#include "Components.hpp"

#include <vector>

namespace Saturn {

class Scene;

struct SceneGraph {
    // temporary
    std::vector<VertexArray*> vtx_arrays;
    std::vector<Components::Transform*> transforms; // temp
    Shader* shader;
    Scene* scene;
};

} // namespace Saturn

#endif
