#ifndef SATURN_ENGINE_HPP_
#define SATURN_ENGINE_HPP

#include <phobos/forward.hpp>

#include <saturn/ecs/system_manager.hpp>

namespace saturn {

class Engine {
public:
    Engine();
    ~Engine();

    void run();

    // This function can be used to plug your own systems into the engine.
    // TODO: refactor this system to make the engine more customizable and easier to plug into
    ecs::system_manager& get_systems() {
        return systems;
    }
    
private:
    ph::WindowContext* window_context;
    ph::VulkanContext* vulkan_context;

    ecs::system_manager systems;
};

} // namespace saturn

#endif