#ifndef SATURN_RIGIDBODY_COMPONENT_HPP_
#define SATURN_RIGIDBODY_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {
class Physics;
}

// clang-format off
namespace Saturn::Components {

struct [[saturn::component, saturn::default_serialize]] Rigidbody
    : public ComponentBase {

	[[saturn::tooltip("The mass of the rigid body.")]]
    float mass;

	[[saturn::tooltip("The axes that should not be affected by the physics system")]]
	glm::bvec3 locked_axes = {false, false, false};
private:
    friend class ::Saturn::Physics;
    glm::vec3 velocity;
};

// clang-format on

} // namespace Saturn::Components

#endif
