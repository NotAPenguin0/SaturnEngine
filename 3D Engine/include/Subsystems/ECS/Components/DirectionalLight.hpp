#ifndef SATURN_DIRECTIONAL_LIGHT_COMPONENT
#define SATURN_DIRECTIONAL_LIGHT_COMPONENT

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE DirectionalLight : public ComponentBase {
    color3 ambient;
    color3 diffuse;
    color3 specular;
    glm::vec3 direction;
};

} // namespace Saturn::Components

#endif
