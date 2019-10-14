#ifndef SATURN_CANVAS_COMPONENT_HPP_
#define SATURN_CANVAS_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {

// There is only one canvas in each scene. If there are more, only the first
// will be used
struct COMPONENT DEFAULT_SERIALIZE Canvas : ComponentBase {
    glm::vec2 position;
    glm::vec2 size;
	float opacity;
};

} // namespace Saturn::Components

#endif // ifndef SATURN_CANVAS_COMPONENT_HPP_
