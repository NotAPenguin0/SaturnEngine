#ifndef MVG_ROTATOR_COMPONENT_HPP_
#define MVG_ROTATOR_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/Math/Math.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Rotator : public ComponentBase {
    float speed;
    glm::vec3 euler_angles;
};

} // namespace Saturn::Components

#endif