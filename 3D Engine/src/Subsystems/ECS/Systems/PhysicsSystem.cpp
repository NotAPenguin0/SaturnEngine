#include "Subsystems/ECS/Systems/PhysicsSystem.hpp"

#include "Subsystems/Scene/Scene.hpp"

#include "Subsystems/ECS/Components/Rigidbody.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

namespace Saturn::Systems {

void PhysicsSystem::on_update(Scene& scene) {
    using namespace Components;

    auto& ecs = scene.get_ecs();
    for (auto [transform, rb] : ecs.select<Transform, Rigidbody>()) {
        // Calculate gravity. Note that we will have to split this up in
        // different functions, classes, files and other modules later
    }
}

} // namespace Saturn::Systems
