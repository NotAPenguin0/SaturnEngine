#ifndef MVG_POINT_LIGHT_HPP_
#define MVG_POINT_LIGHT_HPP_

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE PointLight : public ComponentBase {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float intensity;
};

} // namespace Saturn::Components

#endif