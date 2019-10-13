#ifndef SATURN_PHYSICS_HPP_
#define SATURN_PHYSICS_HPP_

#include <glm/glm.hpp>

#include "AABBCollisionHandler.hpp"
#include "Gravity.hpp"

namespace Saturn {

namespace Components {

struct Rigidbody;
struct Transform;

} // namespace Components

class Scene;

class Physics {
public:
    // This executes a single physics tick.
    // #TODO: Maybe add interpolation for values in the physics system to make
    // everything run smoother
    void tick(Scene& scene);
    // Adds a force to a Rigidbody so it can be further processed by the system
    void add_force(Components::Rigidbody& rb,
                   float force,
                   glm::vec3 const& direction);

private:
    // Applies all currently working forces to the object
    void apply_forces(Components::Transform& transform,
                      Components::Rigidbody& rb);

    Gravity gravity_system;
	AABBCollisionHandler aabb_handler;
};

} // namespace Saturn

#endif
