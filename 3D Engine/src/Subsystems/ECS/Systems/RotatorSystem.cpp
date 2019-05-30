#include "Subsystems/ECS/Systems/RotatorSystem.hpp"

#include "Subsystems/ECS/Components/Rotator.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

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
