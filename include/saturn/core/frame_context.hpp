#ifndef SATURN_FRAME_CONTEXT_HPP_
#define SATURN_FRAME_CONTEXT_HPP_

#include <saturn/scene/scene.hpp>
#include <saturn/ecs/registry.hpp>
#include <phobos/present/frame_info.hpp>

namespace saturn {

struct FrameContext {
    Scene& scene;
    ecs::registry& ecs;
    ph::FrameInfo& render_info;
};  

}

#endif