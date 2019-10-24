#ifndef SATURN_POINT_LIGHT_HPP_
#define SATURN_POINT_LIGHT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] PointLight : public ComponentBase {
   
    [[saturn::tooltip("Ambient light color.")]]
	color3 ambient;

	[[saturn::tooltip("Diffuse light color.")]]
    color3 diffuse;

	[[saturn::tooltip("Specular light color.")]]
    color3 specular;

	[[saturn::tooltip("Light intensity.")]]
    float intensity;
};
// clang-format on

} // namespace Saturn::Components

#endif
