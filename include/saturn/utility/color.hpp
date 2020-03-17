#ifndef SATURN_COLOR_HPP_
#define SATURN_COLOR_HPP_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace saturn {

struct color3 : glm::vec3 {
    using glm::vec3::vec3;
};

struct color4 : glm::vec4 {
    using glm::vec4::vec4;
};

}

#endif