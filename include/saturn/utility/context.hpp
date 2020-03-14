#ifndef SATURN_CONTEXT_HPP_
#define SATURN_CONTEXT_HPP_

#include <phobos/forward.hpp>

namespace saturn {

class Scene;

struct Context {
    ph::VulkanContext* vulkan;
    Scene* scene;
};

}

#endif