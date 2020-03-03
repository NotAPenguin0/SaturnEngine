#ifndef SATURN_COMPONENT_STATIC_MESH_HPP_
#define SATURN_COMPONENT_STATIC_MESH_HPP_

#include <phobos/assets/handle.hpp>
#include <phobos/forward.hpp>

namespace saturn::components {

struct [[component]] StaticMesh {
    ph::Handle<ph::Mesh> mesh;
};

}

#endif