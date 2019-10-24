#ifndef SATURN_COMPONENTS_TRANSFORM_HPP_
#define SATURN_COMPONENTS_TRANSFORM_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {

namespace Components {
// clang-format off
struct [[saturn::component, saturn::default_serialize]] Transform
    : public ComponentBase {

	[[saturn::tooltip("Position of the entity in world space, or relative to its parent.")]]
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    // euler angles rotation
	[[saturn::tooltip("Rotation of the entity in euler angles (relative to parent).")]]
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	[[saturn::tooltip("Scale of the entity, independent of parent.")]]
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
// clang-format on 

} // namespace Components

} // namespace Saturn

#endif
