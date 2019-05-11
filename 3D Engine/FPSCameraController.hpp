#ifndef MVG_FPS_CAMERA_CONTROLLER_HPP_
#define MVG_FPS_CAMERA_CONTROLLER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct FPSCameraController : public ComponentBase {
    float speed;
};

}

#endif