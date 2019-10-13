#ifndef SATURN_STATIC_MESH_COMPONENT_HPP_
#define SATURN_STATIC_MESH_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "AssetManager/Resource.hpp"
#include "Renderer/Mesh.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE StaticMesh : public ComponentBase {
    Resource<Mesh> mesh;
	bool face_cull = true;
};

} // namespace Saturn::Components

#endif
