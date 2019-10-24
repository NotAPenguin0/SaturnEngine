#ifndef SATURN_DIRECTIONAL_LIGHT_COMPONENT
#define SATURN_DIRECTIONAL_LIGHT_COMPONENT

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("Light")]] DirectionalLight
    : public ComponentBase {
    [[tooltip("Ambient light color.")]]
	color3 ambient;

	[[tooltip("Diffuse light color.")]]
    color3 diffuse;

	[[tooltip("Specular light color.")]]
    color3 specular;

	[[tooltip("Light direction vector.")]]
    glm::vec3 direction;
};
// clang-format on

} // namespace Saturn::Components

#endif
