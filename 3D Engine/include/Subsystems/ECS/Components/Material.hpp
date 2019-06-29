#ifndef MVG_MATERIAL_HPP_
#define MVG_MATERIAL_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/Texture.hpp"

namespace Saturn::Components {

struct Material : public ComponentBase {
    Resource<Shader> shader;
	// Optional
	Resource<Texture> texture;

	bool lit = true;
};

} // namespace Saturn::Components

#endif
