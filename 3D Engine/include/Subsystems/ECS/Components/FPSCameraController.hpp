#ifndef MVG_FPS_CAMERA_CONTROLLER_HPP_
#define MVG_FPS_CAMERA_CONTROLLER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE FPSCameraController : public ComponentBase {
	// Movement speed
    float speed;
};

} // namespace Saturn::Components

#endif
