#include "ECS/Systems/FlashlightSystem.hpp"

#include "ECS/Components/Camera.hpp"
#include "ECS/Components/SpotLight.hpp"
#include "Scene/Scene.hpp"

namespace Saturn::Systems {

void FlashlightSystem::on_update(Scene& scene) {
    using namespace Components;

    for (auto [cam, light] : scene.get_ecs().select<Camera, SpotLight>()) {
        light.direction = cam.front;
    }
}

} // namespace Saturn::Systems
