#ifndef SATURN_SOUND_ENGINE_HPP_
#define SATURN_SOUND_ENGINE_HPP_

// Forward declarations
namespace irrklang {
class ISoundEngine;
} // namespace irrklang

namespace Saturn {

class SoundEngine {
public:
    static void init();
    static void destroy();

private:
    static irrklang::ISoundEngine* engine;
};

} // namespace Saturn

#endif