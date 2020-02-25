#ifndef SATURN_COMPONENTS_POINT_LIGHT_HPP_
#define SATURN_COMPONENTS_POINT_LIGHT_HPP_

#include <glm/vec3.hpp>

namespace saturn::components {

struct PointLight {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

}

#endif