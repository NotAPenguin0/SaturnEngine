#ifndef SATURN_TEXT_COMPONENT_HPP_
#define SATURN_TEXT_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include "Utility/Color.hpp"

#include <glm/glm.hpp>
#include <string>

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Text : ComponentBase {
    std::string text;
    glm::vec2 position;
    glm::vec2 size;
	color3 color;
};

} // namespace Saturn::Components

#endif // ifndef SATURN_TEXT_COMPONENT_HPP_
