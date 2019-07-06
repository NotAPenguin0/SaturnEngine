#ifndef MVG_COMPONENT_BASE_HPP_
#define MVG_COMPONENT_BASE_HPP_

#include <cstddef>

#include "Subsystems/Serialization/CodeGenDefinitions.hpp"

namespace Saturn {
class SceneObject;
}

namespace Saturn {

namespace Components {

// Base struct for components. Does nothing more than storing a pointer to the
// owning entity and an id referring to itself
struct ComponentBase {
    SceneObject* entity;
    std::size_t id;
};

} // namespace Components

} // namespace Saturn
#endif
