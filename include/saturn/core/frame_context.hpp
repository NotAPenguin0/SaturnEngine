#ifndef SATURN_FRAME_CONTEXT_HPP_
#define SATURN_FRAME_CONTEXT_HPP_

#include <saturn/ecs/registry.hpp>
#include <phobos/present/frame_info.hpp>

namespace saturn {

struct FrameContext {
    ecs::registry& ecs;
    ph::FrameInfo& render_info;
};  

}

#endif