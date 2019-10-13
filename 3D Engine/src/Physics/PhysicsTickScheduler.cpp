#include "Physics/PhysicsTickScheduler.hpp"

#include "Physics/Physics.hpp"
#include "Scene/Scene.hpp"
#include "Time/Time.hpp"

#include <iostream>

namespace Saturn {

// #Temp: Hardcoded timestep here. This timestep is in seconds. 0.16 means every
// frame at 60 fps
static constexpr float timestep = 0.016f;

PhysicsTickScheduler::PhysicsTickScheduler(Physics& physics_system) :
    physics(&physics_system) {}

PhysicsTickScheduler::PhysicsTickScheduler(PhysicsTickScheduler&& rhs) {
    physics = rhs.physics;
    rhs.physics = nullptr;
}

PhysicsTickScheduler&
PhysicsTickScheduler::operator=(PhysicsTickScheduler&& rhs) {
    physics = rhs.physics;
    rhs.physics = nullptr;

    return *this;
}

void PhysicsTickScheduler::set_physics_system(Physics& physics_system) {
    physics = &physics_system;
}

void PhysicsTickScheduler::update(SystemUpdateMode mode, Scene& scene) {
    // Only update physics system when not in editor mode
    if (mode != SystemUpdateMode::Editor) {
        timer += Time::deltaTime;

        // While we have time left to do a physics time step
        while (timer - timestep >= 0) {
            physics->tick(scene);
            timer -= timestep;
        }
    } else {
        // If we're in editor mode, reset the timer so that it will always be
        // correct when entering play mode
        timer = 0;
    }
}

} // namespace Saturn
