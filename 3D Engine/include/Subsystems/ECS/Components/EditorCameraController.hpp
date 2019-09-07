#ifndef SATURN_EDITOR_CAMERA_CONTROLLER_COMPONENT_HPP_
#define SATURN_EDITOR_CAMERA_CONTROLLER_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

// Controls and Editor Camera
struct COMPONENT DEFAULT_SERIALIZE HIDE_IN_EDITOR EditorCameraController
    : public ComponentBase {
	float speed;
    float sensitivity;
    float zoom_speed;
};

} // namespace Saturn::Components

#endif
