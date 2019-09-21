#ifndef SATURN_SOUND_LISTENER_COMPONENT_HPP_
#define SATURN_SOUND_LISTENER_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

// Every scene must have exactly one SoundListener component
struct COMPONENT DEFAULT_SERIALIZE SoundListener : ComponentBase {
    // position (relative to the entity's position)
    glm::vec3 position;
	// forward vector
	glm::vec3 forward;
};

} // namespace Saturn::Components

#endif
