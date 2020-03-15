#ifndef SATURN_COMPONENT_TRANSFORM_HPP_
#define SATURN_COMPONENT_TRANSFORM_HPP_

#include <glm/vec3.hpp>

namespace saturn {

namespace components {

struct [[component]] Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale = glm::vec3(1, 1, 1);
};

} // namespace components

} // namespace saturn

#endif