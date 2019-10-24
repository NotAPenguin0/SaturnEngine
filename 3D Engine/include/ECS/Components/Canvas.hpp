#ifndef SATURN_CANVAS_COMPONENT_HPP_
#define SATURN_CANVAS_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {

// There is only one canvas in each scene. If there are more, only the first
// will be used
// clang-format off
struct [[component, default_serialize]] Canvas
    : ComponentBase {
    [[tooltip("The position of the canvas in 2D screen coordinates ranging from 0 to 1. Left top is (0, 0).")]] 
	glm::vec2 position;
	[[tooltip("Size of the canvas relative to the screen size.")]]
    glm::vec2 size;
	[[tooltip("Opacity of the UI overlay drawn on this canvas.")]]
    float opacity;
};
// clang-format on

} // namespace Saturn::Components

#endif // ifndef SATURN_CANVAS_COMPONENT_HPP_
