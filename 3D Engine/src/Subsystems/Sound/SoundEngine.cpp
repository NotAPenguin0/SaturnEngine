#include "Subsystems/Sound/SoundEngine.hpp"

#include <irrKlang.h>
#include <stdexcept>

namespace Saturn {

// static variables
irrklang::ISoundEngine* SoundEngine::engine = nullptr;

void SoundEngine::init() {
    // Create the sound engine
    engine = irrklang::createIrrKlangDevice();

    if (!engine) {
        throw std::runtime_error(
            "Failed initialize sound engine: failed to create irrklang device");
    }
}

void SoundEngine::destroy() {
    if (engine) { engine->drop(); }
}

} // namespace Saturn
