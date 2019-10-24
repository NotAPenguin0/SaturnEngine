#ifndef SATURN_SOUND_COMPONENT_HPP_
#define SATURN_SOUND_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <audeo/audeo.hpp>

namespace Saturn::Components {

// clang-format off
struct [[component, category("Sound")]] Sound : ComponentBase {
	[[tooltip("The sound handle this sound component refers to.")]]
    audeo::Sound sound;
};
// clang-format on

} // namespace Saturn::Components

#endif
