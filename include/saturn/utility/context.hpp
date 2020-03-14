#ifndef SATURN_CONTEXT_HPP_
#define SATURN_CONTEXT_HPP_

#include <phobos/forward.hpp>

namespace saturn {

struct Context {
    ph::VulkanContext* vulkan;
};

}

#endif