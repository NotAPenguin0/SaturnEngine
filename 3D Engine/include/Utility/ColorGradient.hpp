#ifndef MVG_COLOR_GRADIENT_HPP_
#define MVG_COLOR_GRADIENT_HPP_

#include <glm/glm.hpp>

namespace Saturn {

class ColorGradient {
public:
    glm::vec4 start;
	glm::vec4 end;

	// x must be in range [0, 1]
	glm::vec4 interpolate(float x);
};

} // namespace Saturn

#endif
