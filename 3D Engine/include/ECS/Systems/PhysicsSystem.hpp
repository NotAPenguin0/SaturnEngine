#ifndef SATURN_PHYSICS_SYSTEM_HPP_
#define SATURN_PHYSICS_SYSTEM_HPP_

#include "SystemBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {
struct Rigidbody;
struct Transform;
} // namespace Saturn::Components

namespace Saturn::Systems {

// #TODO: Run Physics system separately from all other systems, at fixed
// time steps (maybe even multithread it?)
// https://discordapp.com/channels/331718482485837825/552607083330011154/603208251584020481
// ^ Link to conversation with martty in TCCPP explaining how this works.

class PhysicsSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
    // Physics system updates in late update step
    void on_late_update(Scene& scene) override;
    // #TODO: Make this a setting somewhere in the UI maybe ?
    static constexpr float gravity_constant = 9.81f;

private:
    // To soften the effect of the gravity force. Otherwise a mass of 1.0 falls
    // down way too fast
    static constexpr float gravity_multiplier = 0.005f;

    void add_force(struct Components::Rigidbody& rb,
                   float force,
                   glm::vec3 const& direction);

    void apply_forces(struct ::Saturn::Components::Rigidbody& rb,
                      struct ::Saturn::Components::Transform& trans);
};

} // namespace Saturn::Systems

#endif
