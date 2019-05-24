#include "Utility/ColorGradient.hpp"

namespace Saturn {

glm::vec4 ColorGradient::interpolate(float x) {
    return start + (end - start) * x;
}

} // namespace Saturn
