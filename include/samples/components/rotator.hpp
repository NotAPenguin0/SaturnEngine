#ifndef SATURN_ROTATOR_COMPONENT_HPP_
#define SATURN_ROTATOR_COMPONENT_HPP_

#include <glm/vec3.hpp>

namespace saturn::components {

struct [[component]] Rotator {
    float speed = 0.1f;
    glm::vec3 axes = glm::vec3(0, 1, 0);
};

}

#endif