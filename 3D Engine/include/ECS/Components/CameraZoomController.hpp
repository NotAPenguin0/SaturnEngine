#ifndef SATURN_CAMERA_ZOOM_CONTROLLER_HPP_
#define SATURN_CAMERA_ZOOM_CONTROLLER_HPP_

#include "ComponentBase.hpp"
#include "Utility/in_range.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize]] CameraZoomController
    : public ComponentBase {
    [[tooltip("The speed of the zoom. Note that zoom corresponds to FOV.")]] 
	float zoom_speed;

    // minimum zoom. Has to be > 0
	[[tooltip("Minimum zoom in degrees. Always greater than zero.")]]
    float min_zoom = 1.0f;
    // maximum zoom. Has to be < 180.0f
	[[tooltip("Maximum zoom in degrees. Has to be smaller than 180")]]
    float max_zoom = 45.0f;
};
// clang-format on

} // namespace Saturn::Components

#endif
