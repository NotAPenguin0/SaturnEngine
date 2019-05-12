#ifndef MVG_CAMERA_ZOOM_CONTROLLER_HPP_
#define MVG_CAMERA_ZOOM_CONTROLLER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct CameraZoomController : public ComponentBase {
    float zoom_speed;

	//#TODO: Enforce min/max to be in a valid range

	// minimum zoom. Has to be > 0
    float min_zoom = 1.0f;
	// maximum zoom. Has to be < 180.0f
    float max_zoom = 45.0f;
};

} // namespace Saturn::Components

#endif
