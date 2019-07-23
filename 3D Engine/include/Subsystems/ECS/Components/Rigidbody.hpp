#ifndef MVG_RIGIDBODY_COMPONENT_HPP_
#define MVG_RIGIDBODY_COMPONENT_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Rigidbody : public ComponentBase {
    float mass;
};

} // namespace Saturn::Components

#endif
