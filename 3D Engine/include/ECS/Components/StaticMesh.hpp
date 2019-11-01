#ifndef SATURN_STATIC_MESH_COMPONENT_HPP_
#define SATURN_STATIC_MESH_COMPONENT_HPP_

#include "AssetManager/Resource.hpp"
#include "ComponentBase.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Material.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("Rendering")]] StaticMesh
    : public ComponentBase {

	[[tooltip("The mesh resource that will be rendered")]]
    Resource<Mesh> mesh;

	[[tooltip("The material to use when rendering the mesh")]]
	Resource<Material> material;

	[[tooltip("Whether to do face culling on the mesh. Usually you want this, except for planes.")]]
    bool face_cull = true;
};
// clang-format on

} // namespace Saturn::Components

#endif
