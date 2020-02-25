#ifndef SATURN_ENGINE_HPP_
#define SATURN_ENGINE_HPP

#include <phobos/forward.hpp>

namespace saturn {

class Engine {
public:
    Engine();
    ~Engine();

    void run();
    
private:
    ph::WindowContext* window_context;
    ph::VulkanContext* vulkan_context;
};

} // namespace saturn

#endif