#ifndef SATURN_FPS_SOUND_LISTENER_SYSTEM_HPP_
#define SATURN_FPS_SOUND_LISTENER_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Systems {

class SYSTEM FPSSoundListenerSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif
