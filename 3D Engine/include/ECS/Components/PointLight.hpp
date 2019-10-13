#ifndef SATURN_POINT_LIGHT_HPP_
#define SATURN_POINT_LIGHT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE PointLight : public ComponentBase {
    color3 ambient;
    color3 diffuse;
    color3 specular;
    float intensity;
};

} // namespace Saturn::Components

#endif
