#ifndef SATURN_SOUND_LISTENER_COMPONENT_HPP_
#define SATURN_SOUND_LISTENER_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

// Every scene must have exactly one SoundListener component
// clang-format off
struct [[component, default_serialize]] SoundListener
    : ComponentBase {
    // position (relative to the entity's position)
	[[tooltip("Position of the sound listener, relatieve to the entity position")]]
    glm::vec3 position;
    // forward vector
	[[tooltip("The forward vector of the listener")]]
    glm::vec3 forward;
};
// clang-format n

} // namespace Saturn::Components

#endif
