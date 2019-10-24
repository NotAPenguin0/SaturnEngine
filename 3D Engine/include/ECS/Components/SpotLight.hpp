#ifndef SATURN_SPOT_LIGHT_COMPONENT_HPP_
#define SATURN_SPOT_LIGHT_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("Light")]] SpotLight
    : ComponentBase {

    [[tooltip("Ambient light color.")]]
	color3 ambient;

	[[tooltip("Diffuse light color.")]]
    color3 diffuse;

	[[tooltip("Specular light color.")]]
    color3 specular;

	[[tooltip("Directin in which the light shines.")]]
    glm::vec3 direction;
	
	[[tooltip("The inner angle of the spot light. "
		"This is where the intensity will begin to fade out. (in degrees).")]]
    float inner_angle; // in degrees

		[[tooltip("The inner angle of the spot light. "
		"This is where the intensity will be completely faded out. (in degrees).")]]
    float outer_angle; // in degrees

	[[tooltip("Light intensity.")]]
    float intensity;
};
// clang-format on

} // namespace Saturn::Components

#endif
