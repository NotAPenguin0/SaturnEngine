#ifndef SATURN_COMPONENT_BASE_HPP_
#define SATURN_COMPONENT_BASE_HPP_

#include "Serialization/CodeGenDefinitions.hpp"
#include "Serialization/ForwardDeclarations.hpp"

namespace Saturn {
class SceneObject;
}

namespace Saturn {

namespace Components {

// Base struct for components. Does nothing more than storing a pointer to the
// owning entity and an id referring to itself
struct ComponentBase {
    SceneObject* entity;
    unsigned int id;
};

} // namespace Components

} // namespace Saturn
#endif
