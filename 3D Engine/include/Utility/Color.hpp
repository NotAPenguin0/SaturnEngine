#ifndef SATURN_COLOR_HPP_
#define SATURN_COLOR_HPP_

#include <glm/glm.hpp>

namespace Saturn {

struct color3 : public glm::vec3 {
    using glm::vec3::vec3;
};

struct color4 : glm::vec4 {
    using glm::vec4::vec4;
};

} // namespace Saturn

#endif
