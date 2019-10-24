#ifndef SATURN_ROTATOR_COMPONENT_HPP_
#define SATURN_ROTATOR_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("User-defined")]] Rotator
    : public ComponentBase {

	[[tooltip("Speed at which to rotate")]]
    float speed;
    
	[[tooltip("The angles around which to rotate (in degrees).")]]
	glm::vec3 euler_angles;
};
// clang-format on

} // namespace Saturn::Components

#endif
