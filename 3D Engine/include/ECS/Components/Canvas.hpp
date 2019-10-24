#ifndef SATURN_CANVAS_COMPONENT_HPP_
#define SATURN_CANVAS_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn::Components {

// There is only one canvas in each scene. If there are more, only the first
// will be used
// clang-format off
struct [[saturn::component, saturn::default_serialize]] Canvas
    : ComponentBase {
    [[saturn::tooltip("The position of the canvas in 2D screen coordinates ranging from 0 to 1. Left top is (0, 0).")]] 
	glm::vec2 position;
	[[saturn::tooltip("Size of the canvas relative to the screen size.")]]
    glm::vec2 size;
	[[saturn::tooltip("Opacity of the UI overlay drawn on this canvas.")]]
    float opacity;
};
// clang-format on

} // namespace Saturn::Components

#endif // ifndef SATURN_CANVAS_COMPONENT_HPP_
