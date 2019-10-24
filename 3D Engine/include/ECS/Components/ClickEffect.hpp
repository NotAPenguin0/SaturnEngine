#ifndef SATURN_CLICK_EFFECT_COMPONENT_HPP_
#define SATURN_CLICK_EFFECT_COMPONENT_HPP_

// This component is part of a user script. It is not part of the main engine.

#include "AssetManager/Resource.hpp"
#include "ComponentBase.hpp"

#include <audeo/audeo.hpp>

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] ClickEffect
    : ComponentBase {
    [[saturn::tooltip("Sound effect to play on mouse click.")]] 
	Resource<audeo::SoundSource> sound_effect;
	[[saturn::tooltip("The maximum distance the effect can be heard from.")]]
    float max_distance = 10.0f;
};
// clang-format on

} // namespace Saturn::Components

#endif
