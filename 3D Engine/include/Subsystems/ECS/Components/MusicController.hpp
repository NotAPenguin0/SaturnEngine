#ifndef SATURN_MUSIC_CONTROLLER_COMPONENT_HPP_
#define SATURN_MUSIC_CONTROLLER_COMPONENT_HPP_

// This component is part of a user script. It is not part of the main engine.

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"

#include <audeo/audeo.hpp>

namespace Saturn::Systems {
	class MusicSystem;
}

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE MusicController : ComponentBase {
    friend class Systems::MusicSystem;

    Resource<audeo::SoundSource> music;
    bool loop;
    float fade_in_ms;

private:
    bool is_playing = false;
};

} // namespace Saturn::Components

#endif
