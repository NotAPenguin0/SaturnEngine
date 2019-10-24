#ifndef SATURN_COMPONENT_CAMERA_HPP_
#define SATURN_COMPONENT_CAMERA_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

namespace Saturn {

namespace Components {
// clang-format off
struct [[component, default_serialize, category("Camera")]] Camera
    : public ComponentBase {
    [[tooltip("Front vector of the camera.")]] 
	glm::vec3 front;
    [[tooltip("Up vector of the camera.")]] 
	glm::vec3 up;

    [[tooltip("Camera field of view (in degrees).")]] 
	float fov;
    [[tooltip("Id of the viewport that this camera is assigned to.")]] 
	unsigned int viewport_id = 0;
};
// clang-format on

} // namespace Components

} // namespace Saturn

#endif
