#ifndef SATURN_COLOR_GRADIENT_HPP_
#define SATURN_COLOR_GRADIENT_HPP_

#include <glm/glm.hpp>

namespace Saturn {

class ColorGradient {
public:
    glm::vec4 start = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 end = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// x must be in range [0, 1]
	glm::vec4 interpolate(float x);
};

} // namespace Saturn

#endif
