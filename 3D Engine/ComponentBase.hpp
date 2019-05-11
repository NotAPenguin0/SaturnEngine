#ifndef MVG_COMPONENT_BASE_HPP_
#define MVG_COMPONENT_BASE_HPP_

#include <cstddef>

namespace Saturn {
class SceneObject;
}

namespace Saturn::Components {

// Base struct for components. Does nothing more than storing a pointer to the
// owning entity
struct ComponentBase {

	ComponentBase() = default;
	ComponentBase(ComponentBase const&) = default;
	ComponentBase(ComponentBase&&) = default;
	
	ComponentBase& operator=(ComponentBase const&) = default;
	ComponentBase& operator=(ComponentBase&&) = default;

    SceneObject* entity;
    std::size_t id;
};

} // namespace Saturn::Components
#endif
