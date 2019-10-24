#ifndef SATURN_MUSIC_CONTROLLER_COMPONENT_HPP_
#define SATURN_MUSIC_CONTROLLER_COMPONENT_HPP_

// This component is part of a user script. It is not part of the main engine.

#include "AssetManager/Resource.hpp"
#include "ComponentBase.hpp"

#include <audeo/audeo.hpp>

namespace Saturn::Systems {
class MusicSystem;
}

namespace Saturn::Components {
// clang-format off
struct [[component, default_serialize, category("Sound")]] MusicController
    : ComponentBase {
    friend class Systems::MusicSystem;

	[[tooltip("The music source to play.")]]
    Resource<audeo::SoundSource> music;

	[[tooltip("Whether to loop the music when it has finished playing.")]]
    bool loop;

	[[tooltip("The amount of ms to fade in the music when starting playback.")]]
    float fade_in_ms;

private:
    bool is_playing = false;
};
// clang-format on

} // namespace Saturn::Components

#endif
