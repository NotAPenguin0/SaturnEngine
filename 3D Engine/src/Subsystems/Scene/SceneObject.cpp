#include "Subsystems\Scene\SceneObject.hpp"
#include "Subsystems\Scene\Scene.hpp"

#include "Subsystems\AssetManager\AssetManager.hpp"

#include <vector>

namespace Saturn {

SceneObject::SceneObject(Scene* s, SceneObject* parent /*= nullptr*/) :
    scene(s), parent_obj(parent) {
    // Debug:

    debug.shader =
        AssetManager<Shader>::get_resource("resources/shaders/default.sh");
}

bool SceneObject::has_parent() const { return parent_obj != nullptr; }

SceneObject* SceneObject::parent() { return parent_obj; }

SceneObject const* SceneObject::parent() const { return parent_obj; }

} // namespace Saturn
