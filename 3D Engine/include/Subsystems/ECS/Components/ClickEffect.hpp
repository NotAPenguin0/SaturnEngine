#ifndef SATURN_CLICK_EFFECT_COMPONENT_HPP_
#define SATURN_CLICK_EFFECT_COMPONENT_HPP_

// This component is part of a user script. It is not part of the main engine.

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"

#include <audeo/audeo.hpp>

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE ClickEffect : ComponentBase {
    Resource<audeo::SoundSource> sound_effect;
};

} // namespace Saturn::Components

#endif
