#include "ECS/Systems/RotatorSystem.hpp"

#include "ECS/Components/Rotator.hpp"
#include "ECS/Components/Transform.hpp"

#include "Scene/Scene.hpp"
#include "Time/Time.hpp"

namespace Saturn::Systems {

void RotatorSystem::on_update(Scene& scene) {
    using namespace Components;
    for (auto [transform, rotator] :
         scene.get_ecs().select<Transform, Rotator>()) {

        transform.rotation +=
            rotator.euler_angles * rotator.speed * Time::deltaTime;
    }
}

} // namespace Saturn::Systems
