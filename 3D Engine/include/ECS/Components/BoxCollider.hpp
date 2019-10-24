#ifndef SATURN_BOX_COLLIDER_COMPONENT_HPP_
#define SATURN_BOX_COLLIDER_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {
// clang-format off
struct [[component, default_serialize]] BoxCollider
    : ComponentBase {
    // The center of the collider. In object space
    [[tooltip("The center of the collider.")]] 
	glm::vec3 center;

    [[tooltip("Half the width of the box for each axis.")]] 
	glm::vec3 half_widths;
};
// clang-format on

} // namespace Saturn::Components

#endif
