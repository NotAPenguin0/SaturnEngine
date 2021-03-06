#include <saturn/ecs/system_manager.hpp>

namespace saturn::ecs {

void system_manager::startup(ph::VulkanContext& ctx, Scene& scene) {
    for (auto& sys : systems) {
        sys->startup(ctx, scene);
    }
}

void system_manager::update_all(saturn::FrameContext& ctx) {
    for (auto& sys : systems) {
        sys->update(ctx);
    }
}

}