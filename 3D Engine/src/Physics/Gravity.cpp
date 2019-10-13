#include "Physics/Gravity.hpp"

#include "ECS/Components/Rigidbody.hpp"

#include "Physics/Physics.hpp"
#include "Time/Time.hpp"

namespace Saturn {

using namespace Components;

// To soften the effect of the gravity force. Otherwise a mass of 1.0 falls
// down way too fast
static constexpr float gravity_multiplier = 0.00064f;

void Gravity::add_gravity(Physics& physics, Rigidbody& rb) {

    float gravity = rb.mass * gravity_constant * gravity_multiplier;

    physics.add_force(rb, gravity, glm::vec3{0.0f, -1.0f, 0.0f});
}

} // namespace Saturn
