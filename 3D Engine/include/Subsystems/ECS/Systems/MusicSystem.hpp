#ifndef SATURN_MUSIC_SYSTEM_HPP_
#define SATURN_MUSIC_SYSTEM_HPP_

#include "SystemBase.hpp"

// This system is part of a user script. It is not part of the main engine.

namespace Saturn::Systems {

class SYSTEM MusicSystem : public SystemBase {
public:
	void on_start(Scene& scene) override;
    void on_update(Scene& scene) override;
	void on_exit(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif
