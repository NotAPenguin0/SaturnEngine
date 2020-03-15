#ifndef SATURN_SAMPLE_ROTATOR_SYSTEM_HPP_
#define SATURN_SAMPLE_ROTATOR_SYSTEM_HPP_

#include <saturn/systems/system.hpp>

namespace samples {

class RotatorSystem : public saturn::systems::System {
public:
    void update(saturn::FrameContext& ctx) override;
};

}

#endif