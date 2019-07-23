#ifndef MVG_RIGIDBODY_COMPONENT_HPP_
#define MVG_RIGIDBODY_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {
namespace Systems {
class PhysicsSystem;
}
}

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Rigidbody : public ComponentBase {
    float mass;
	glm::bvec3 locked_axes = {false, false, false};
private:
    friend class ::Saturn::Systems::PhysicsSystem;
    glm::vec3 forces;
};

} // namespace Saturn::Components

#endif
