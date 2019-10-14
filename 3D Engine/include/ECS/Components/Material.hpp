#ifndef SATURN_MATERIAL_HPP_
#define SATURN_MATERIAL_HPP_

#include "ComponentBase.hpp"
#include "AssetManager/Resource.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Material : public ComponentBase {
    Resource<Shader> shader;
    // Optional
    /*Resource<Texture> texture;*/
    bool lit = true;

    // Lighting data
    Resource<Texture> diffuse_map;
    Resource<Texture> specular_map;
	Resource<Texture> normal_map;
	float shininess;
};

} // namespace Saturn::Components

#endif