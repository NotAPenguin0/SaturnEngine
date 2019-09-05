#ifndef SATURN_SPOT_LIGHT_COMPONENT_HPP_
#define SATURN_SPOT_LIGHT_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE SpotLight : ComponentBase {
    color3 ambient;
    color3 diffuse;
    color3 specular;
    glm::vec3 direction;
    float inner_angle; // in degrees
    float outer_angle; // in degrees
    float intensity;
};

} // namespace Saturn::Components

#endif
