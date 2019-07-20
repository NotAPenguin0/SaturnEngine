#ifndef MVG_STATIC_MESH_COMPONENT_HPP_
#define MVG_STATIC_MESH_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Mesh.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE StaticMesh : public ComponentBase {
    Resource<Mesh> mesh;
	bool face_cull = true;
};

} // namespace Saturn::Components

#endif
