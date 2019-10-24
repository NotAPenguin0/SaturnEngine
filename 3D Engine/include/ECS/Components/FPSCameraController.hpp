#ifndef SATURN_FPS_CAMERA_CONTROLLER_HPP_
#define SATURN_FPS_CAMERA_CONTROLLER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {
// clang-format off
struct [[component, default_serialize]] FPSCameraController
    : public ComponentBase {
    // Movement speed
	[[tooltip("The movement speed")]]
    float speed;
};
// clang-format on

} // namespace Saturn::Components

#endif
