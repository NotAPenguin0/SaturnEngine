#ifndef SATURN_EDITOR_CAMERA_CONTROLLER_COMPONENT_HPP_
#define SATURN_EDITOR_CAMERA_CONTROLLER_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include <glm/glm.hpp>

namespace Saturn::Components {

// Controls and Editor Camera
struct [[saturn::component,
         saturn::default_serialize,
         saturn::hide_in_editor]] EditorCameraController
    : public ComponentBase {
    float speed;
    float sensitivity;
    float zoom_speed;

    bool invert_x;
    bool invert_y;
};

} // namespace Saturn::Components

#endif
