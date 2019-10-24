#ifndef SATURN_COMPONENT_FREELOOK_CONTROLLER_HPP_
#define SATURN_COMPONENT_FREELOOK_CONTROLLER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize]] FreeLookController
    : public ComponentBase {
	[[tooltip("The sensitivity of the mouse controller")]]
    float mouse_sensitivity;
};
// clang-format on

} // namespace Saturn::Components

#endif
