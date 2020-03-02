#ifndef SATURN_COMPONENTS_POINT_LIGHT_HPP_
#define SATURN_COMPONENTS_POINT_LIGHT_HPP_

#include <glm/vec3.hpp>

namespace saturn::components {

struct [[component]] PointLight {
    [[editor::color]] glm::vec3 ambient;
    [[editor::color]] glm::vec3 diffuse;
    [[editor::color]] glm::vec3 specular;
};

}

#endif