#ifndef SATURN_SOUND_SYSTEM_HPP_
#define SATURN_SOUND_SYSTEM_HPP_

// The sound system is responsible for:
// - Updating position data for positional sounds

#include "SystemBase.hpp"

namespace Saturn::Systems {

class SYSTEM SoundSystem : public SystemBase {
public:
	void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif
