#ifndef SATURN_SYSTEM_HPP_
#define SATURN_SYSTEM_HPP_

#include <saturn/core/frame_context.hpp>

namespace saturn::systems {

class System {
public:
    System() = default;
    virtual ~System() = default;

    virtual void update(FrameContext& ctx) = 0;
};

}

#endif