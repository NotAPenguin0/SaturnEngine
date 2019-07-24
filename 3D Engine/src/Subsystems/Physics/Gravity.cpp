#include "Subsystems/Physics/Gravity.hpp"

#include "Subsystems/ECS/Components/Rigidbody.hpp"

#include "Subsystems/Time/Time.hpp"
#include "Subsystems/Physics/Physics.hpp"

namespace Saturn {

using namespace Components;

void Gravity::add_gravity(Physics& physics, Rigidbody& rb) {

    float gravity =
        rb.mass * gravity_constant * gravity_multiplier * Time::deltaTime;

    physics.add_force(rb, gravity, glm::vec3{0.0f, -1.0f, 0.0f});
}

} // namespace Saturn