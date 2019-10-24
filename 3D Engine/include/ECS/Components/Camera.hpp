#ifndef SATURN_COMPONENT_CAMERA_HPP_
#define SATURN_COMPONENT_CAMERA_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {

namespace Components {
// clang-format off
struct [[saturn::component, saturn::default_serialize]] Camera
    : public ComponentBase {
    [[saturn::tooltip("Front vector of the camera.")]] 
	glm::vec3 front;
    [[saturn::tooltip("Up vector of the camera.")]] 
	glm::vec3 up;

    [[saturn::tooltip("Camera field of view (in degrees).")]] 
	float fov;
    [[saturn::tooltip("Id of the viewport that this camera is assigned to.")]] 
	unsigned int viewport_id = 0;
};
// clang-format on

} // namespace Components

} // namespace Saturn

#endif
