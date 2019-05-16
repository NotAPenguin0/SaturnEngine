#ifndef MVG_COMPONENTS_TRANSFORM_HPP_
#define MVG_COMPONENTS_TRANSFORM_HPP_

#include "ComponentBase.hpp"

#include "Subsystems/Math/Math.hpp"

namespace Saturn {

namespace Components {

struct Transform : public ComponentBase {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    // euler angles rotation
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

} // namespace Components

} // namespace Saturn

#endif
