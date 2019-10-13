#ifndef SATURN_COMPONENT_CAMERA_HPP_
#define SATURN_COMPONENT_CAMERA_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {

namespace Components {

struct COMPONENT DEFAULT_SERIALIZE Camera : public ComponentBase {
    glm::vec3 front;
    glm::vec3 up;

	float fov;
	unsigned int viewport_id = 0;
};

} // namespace Components

} // namespace Saturn

#endif
