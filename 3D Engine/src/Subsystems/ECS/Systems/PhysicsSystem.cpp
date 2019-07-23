#include "Subsystems/ECS/Systems/PhysicsSystem.hpp"

#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

#include "Subsystems/ECS/Components/Rigidbody.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

namespace Saturn::Systems {
using namespace Components;

void PhysicsSystem::on_update(Scene& scene) {}

void PhysicsSystem::on_late_update(Scene& scene) {

    auto& ecs = scene.get_ecs();
    for (auto [transform, rb] : ecs.select<Transform, Rigidbody>()) {
        // Calculate gravity. Note that we will have to split this up in
        // different functions, classes, files and other modules later

        // See explanation above gravity_multiplier variable in
        // PhysicsSystem.hpp to see why we do this
        float gravity = rb.mass * gravity_constant * gravity_multiplier;
        add_force(rb, gravity, glm::vec3{0.0f, -1.0f, 0.0f});

        // Apply all forces to the object
        apply_forces(rb, transform);
    }
}

void PhysicsSystem::add_force(Rigidbody& rb,
                              float force,
                              glm::vec3 const& direction) {

    rb.forces += force * direction * Time::deltaTime;
}

void PhysicsSystem::apply_forces(Rigidbody& rb, Transform& trans) {
    // Make sure locked axes remain locked. For this we have to negate the
    // locked_axis bool vec, because lock = true has to become 0.0 as a float
    // vector
    rb.forces = rb.forces * static_cast<glm::vec3>(glm::negate(rb.locked_axes));

    trans.position += rb.forces;
}

} // namespace Saturn::Systems
