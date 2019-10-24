#ifndef SATURN_SPOT_LIGHT_COMPONENT_HPP_
#define SATURN_SPOT_LIGHT_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] SpotLight
    : ComponentBase {

    [[saturn::tooltip("Ambient light color.")]]
	color3 ambient;

	[[saturn::tooltip("Diffuse light color.")]]
    color3 diffuse;

	[[saturn::tooltip("Specular light color.")]]
    color3 specular;

	[[saturn::tooltip("Directin in which the light shines.")]]
    glm::vec3 direction;
	
	[[saturn::tooltip("The inner angle of the spot light. "
		"This is where the intensity will begin to fade out. (in degrees).")]]
    float inner_angle; // in degrees

		[[saturn::tooltip("The inner angle of the spot light. "
		"This is where the intensity will be completely faded out. (in degrees).")]]
    float outer_angle; // in degrees

	[[saturn::tooltip("Light intensity.")]]
    float intensity;
};
// clang-format on

} // namespace Saturn::Components

#endif
