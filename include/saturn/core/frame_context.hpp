#ifndef SATURN_FRAME_CONTEXT_HPP_
#define SATURN_FRAME_CONTEXT_HPP_

#include <saturn/scene/scene.hpp>
#include <saturn/ecs/registry.hpp>
#include <phobos/present/frame_info.hpp>

#include <phobos/forward.hpp>

namespace saturn {

struct FrameContext {
    ph::VulkanContext* vulkan;
    Scene& scene;
    ecs::registry& ecs;
    ph::FrameInfo& render_info;
    float delta_time = 0;
};  

}

#endif