#ifndef SATURN_ECS_SYSTEMS_HPP_
#define SATURN_ECS_SYSTEMS_HPP_

#include <saturn/core/frame_context.hpp>
#include <stl/vector.hpp>
#include <stl/unique_ptr.hpp>

#include <saturn/systems/system.hpp>

namespace saturn::ecs {

class system_manager {
public:
    template<typename S, typename... Args>
    void add_system(Args&&... args);

    void update_all(saturn::FrameContext& ctx);

private:
    stl::vector<stl::unique_ptr<systems::System>> systems;
};

template<typename S, typename... Args>
void system_manager::add_system(Args&&... args) {
    systems.push_back(stl::make_unique<S>(stl::forward<Args>(args) ...));
}

}

#endif