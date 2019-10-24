#ifndef SATURN_ROTATOR_COMPONENT_HPP_
#define SATURN_ROTATOR_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] Rotator
    : public ComponentBase {

	[[saturn::tooltip("Speed at which to rotate")]]
    float speed;
    
	[[saturn::tooltip("The angles around which to rotate (in degrees).")]]
	glm::vec3 euler_angles;
};
// clang-format on

} // namespace Saturn::Components

#endif
