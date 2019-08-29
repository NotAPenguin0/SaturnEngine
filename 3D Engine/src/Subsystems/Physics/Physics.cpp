#include "Subsystems/Physics/Physics.hpp"

#include "Subsystems/ECS/Components/Rigidbody.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

#include "Subsystems/Physics/CollisionDetectors.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

namespace Saturn {
using namespace Components;

void Physics::tick(Scene& scene) {
    auto& ecs = scene.get_ecs();
    for (auto [transform, rb] : ecs.select<Transform, Rigidbody>()) {
        // Process gravity
        gravity_system.add_gravity(*this, rb);
    }

    // Process collisions. We will move this to another class later
    for (auto [collider, rb, trans] :
         ecs.select<BoxCollider, Rigidbody, Transform>()) {
        for (auto [collider2, trans2] : ecs.select<BoxCollider, Transform>()) {
            // Do not test for intersection with self
            if (trans.entity == trans2.entity) { continue; }
            auto collision =
                aabb_handler.detect_collision(rb, collider, collider2);
            if (collision.collided) { 
				rb.velocity = glm::vec3(0.0f, 0.0f, 0.0f); 
			}
        }
    }

    for (auto [transform, rb] : ecs.select<Transform, Rigidbody>()) {
        // Apply all accumulated forces to the object
        apply_forces(transform, rb);
    }
}

void Physics::add_force(Rigidbody& rb,
                        float force,
                        glm::vec3 const& direction) {

    rb.velocity += force * direction;
}

void Physics::apply_forces(Transform& trans, Rigidbody& rb) {
    // Make sure locked axes remain locked. For this we have to negate the
    // locked_axis bool vec, because lock = true has to become 0.0 as a float
    // vector
    rb.velocity = rb.velocity * static_cast<glm::vec3>(glm::negate(rb.locked_axes));

    trans.position += rb.velocity;
}

} // namespace Saturn
