#ifndef SATURN_GRAVITY_HPP_
#define SATURN_GRAVITY_HPP_

#include <glm/glm.hpp>

namespace Saturn {

namespace Components {
struct Rigidbody;
}

class Physics;

class Gravity {
public:
    void add_gravity(Physics& physics, Components::Rigidbody& rb);

    static constexpr glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    // Gravity constant. #TODO: We should make this a setting somewhere.
    // I will do this when working on the editor
    static constexpr float gravity_constant = 1.0f;

};

} // namespace Saturn

#endif
