#ifndef MVG_SPOT_LIGHT_COMPONENT_HPP_
#define MVG_SPOT_LIGHT_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

struct SpotLight : ComponentBase {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    float inner_angle; // in degrees
    float outer_angle; // in degrees
    float intensity;
};

} // namespace Saturn::Components

#endif
