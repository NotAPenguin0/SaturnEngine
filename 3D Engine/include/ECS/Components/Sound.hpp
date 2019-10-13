#ifndef SATURN_SOUND_COMPONENT_HPP_
#define SATURN_SOUND_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include <audeo/audeo.hpp>

namespace Saturn::Components {

struct COMPONENT HIDE_IN_EDITOR Sound : ComponentBase {
    audeo::Sound sound;
};

} // namespace Saturn::Components

#endif
