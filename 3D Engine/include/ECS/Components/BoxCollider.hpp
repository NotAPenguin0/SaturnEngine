#ifndef SATURN_BOX_COLLIDER_COMPONENT_HPP_
#define SATURN_BOX_COLLIDER_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE BoxCollider : ComponentBase {
	// The center of the collider. In object space
    glm::vec3 center;
    glm::vec3 half_widths;
};

} // namespace Saturn::Components

#endif