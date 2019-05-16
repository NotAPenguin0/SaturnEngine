#ifndef MVG_SCENE_GRAPH_HPP_
#define MVG_SCENE_GRAPH_HPP_

#include "SubSystems\Renderer\Shader.hpp"
#include "Subsystems/Renderer/Mesh.hpp"
#include "Subsystems\AssetManager\Resource.hpp"

#include "Subsystems\ECS\Components.hpp"

#include <vector>

namespace Saturn {

class Scene;

struct SceneGraph {
    // temporary
    std::vector<Resource<Mesh>> meshes;
    std::vector<Components::Transform*> transforms; // temp
    Resource<Shader> shader;
    Scene* scene;
};

} // namespace Saturn

#endif
