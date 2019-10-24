#ifndef SATURN_DIRECTIONAL_LIGHT_COMPONENT
#define SATURN_DIRECTIONAL_LIGHT_COMPONENT

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] DirectionalLight
    : public ComponentBase {
    [[saturn::tooltip("Ambient light color.")]]
	color3 ambient;

	[[saturn::tooltip("Diffuse light color.")]]
    color3 diffuse;

	[[saturn::tooltip("Specular light color.")]]
    color3 specular;

	[[saturn::tooltip("Light direction vector.")]]
    glm::vec3 direction;
};
// clang-format on

} // namespace Saturn::Components

#endif
