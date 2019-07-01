#ifndef MVG_DIRECTIONAL_LIGHT_COMPONENT
#define MVG_DIRECTIONAL_LIGHT_COMPONENT

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

struct DirectionalLight : public ComponentBase {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
};

} // namespace Saturn::Components

#endif
