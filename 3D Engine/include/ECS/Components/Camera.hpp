#ifndef SATURN_COMPONENT_CAMERA_HPP_
#define SATURN_COMPONENT_CAMERA_HPP_

#include "ComponentBase.hpp"

#include <glm/glm.hpp>

#include "AssetManager/Resource.hpp"
#include "Renderer/CubeMap.hpp"

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
    
	[[tooltip("The skybox to render with this camera")]]
	Resource<CubeMap> skybox;

	[[hide_in_editor]] 
	unsigned int viewport_id = 0;
};
// clang-format on

} // namespace Components

} // namespace Saturn

#endif
