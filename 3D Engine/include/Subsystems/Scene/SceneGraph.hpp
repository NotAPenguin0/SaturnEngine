#ifndef MVG_SCENE_GRAPH_HPP_
#define MVG_SCENE_GRAPH_HPP_

#include "Subsystems\AssetManager\Resource.hpp"
#include "SubSystems\Renderer\Shader.hpp"
#include "Subsystems\Renderer\VertexArray.hpp"

#include "Subsystems\ECS\Components.hpp"

#include <vector>

namespace Saturn {

class Scene;

struct SceneGraph {
    // temporary
    std::vector<VertexArray*> vtx_arrays;
    std::vector<Components::Transform*> transforms; // temp
    Resource<Shader> shader;
    Scene* scene;
};

} // namespace Saturn

#endif
