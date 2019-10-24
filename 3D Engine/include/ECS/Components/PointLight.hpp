#ifndef SATURN_POINT_LIGHT_HPP_
#define SATURN_POINT_LIGHT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("Light")]] PointLight : public ComponentBase {
   
    [[tooltip("Ambient light color.")]]
	color3 ambient;

	[[tooltip("Diffuse light color.")]]
    color3 diffuse;

	[[tooltip("Specular light color.")]]
    color3 specular;

	[[tooltip("Light intensity.")]]
    float intensity;
};
// clang-format on

} // namespace Saturn::Components

#endif
