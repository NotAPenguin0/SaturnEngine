#ifndef SATURN_COMPONENT_STATIC_MESH_HPP_
#define SATURN_COMPONENT_STATIC_MESH_HPP_

#include <saturn/utility/handle.hpp>
#include <phobos/forward.hpp>

namespace saturn::components {

struct [[component]] StaticMesh {
    Handle<ph::Mesh> mesh;
};

}

#endif