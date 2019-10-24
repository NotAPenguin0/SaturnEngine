#ifndef SATURN_BOX_COLLIDER_COMPONENT_HPP_
#define SATURN_BOX_COLLIDER_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {
// clang-format off
struct [[saturn::component, saturn::default_serialize]] BoxCollider
    : ComponentBase {
    // The center of the collider. In object space
    [[saturn::tooltip("The center of the collider.")]] 
	glm::vec3 center;

    [[saturn::tooltip("Half the width of the box for each axis.")]] 
	glm::vec3 half_widths;
};
// clang-format on

} // namespace Saturn::Components

#endif
