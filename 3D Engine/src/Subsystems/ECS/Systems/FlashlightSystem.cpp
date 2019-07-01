#include "Subsystems/ECS/Systems/FlashlightSystem.hpp"

#include "Subsystems/ECS/Components/Camera.hpp"
#include "Subsystems/ECS/Components/SpotLight.hpp"
#include "Subsystems/Scene/Scene.hpp"

namespace Saturn::Systems {

void FlashlightSystem::on_update(Scene& scene) {
    using namespace Components;

    for (auto [cam, light] : scene.get_ecs().select<Camera, SpotLight>()) {
        light.direction = cam.front;
    }
}

} // namespace Saturn::Systems
