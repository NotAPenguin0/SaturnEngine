#ifndef SATURN_MESH_RENDERER_COMPONENT_HPP_
#define SATURN_MESH_RENDERER_COMPONENT_HPP_

#include <phobos/forward.hpp>
#include <saturn/utility/handle.hpp>

namespace saturn::components {

struct [[component]] MeshRenderer {
    Handle<ph::Material> material;
};

}

#endif