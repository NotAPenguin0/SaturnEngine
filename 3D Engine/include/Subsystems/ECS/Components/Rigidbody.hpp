#ifndef SATURN_RIGIDBODY_COMPONENT_HPP_
#define SATURN_RIGIDBODY_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {
class Physics;
}

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Rigidbody : public ComponentBase {
    float mass;
	glm::bvec3 locked_axes = {false, false, false};
private:
    friend class ::Saturn::Physics;
    glm::vec3 velocity;
};

} // namespace Saturn::Components

#endif
