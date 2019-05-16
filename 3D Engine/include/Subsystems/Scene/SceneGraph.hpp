#ifndef MVG_SCENE_GRAPH_HPP_
#define MVG_SCENE_GRAPH_HPP_

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

#include <vector>

namespace Saturn {

class Scene;

struct SceneGraph {
    Resource<Shader> shader; // Temporary
    Scene* scene;
};

} // namespace Saturn

#endif
