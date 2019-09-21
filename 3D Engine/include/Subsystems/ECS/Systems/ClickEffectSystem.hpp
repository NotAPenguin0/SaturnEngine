#ifndef SATURN_CLICK_EFFECT_SYSTEM_HPP_
#define SATURN_CLICK_EFFECT_SYSTEM_HPP_

#include "SystemBase.hpp"

// This system is part of a user script. It is not part of the main engine.

namespace Saturn::Systems {

class SYSTEM ClickEffectSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif
