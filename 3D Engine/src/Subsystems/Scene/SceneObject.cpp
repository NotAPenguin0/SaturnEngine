#include "Subsystems\Scene\SceneObject.hpp"
#include "Subsystems\Scene\Scene.hpp"

#include "Subsystems\AssetManager\AssetManager.hpp"

#include <vector>

namespace Saturn {

SceneObject::SceneObject(Scene* s) : scene(s) {
    // Debug:

    debug.shader =
        AssetManager<Shader>::get_resource("resources/shaders/default.sh");
}

} // namespace Saturn
