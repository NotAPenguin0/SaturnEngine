#ifndef SATURN_MATERIAL_HPP_
#define SATURN_MATERIAL_HPP_

#include "AssetManager/Resource.hpp"
#include "ComponentBase.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("Rendering")]] Material
    : public ComponentBase {

	[[tooltip("The shader this material will use.")]]
    Resource<Shader> shader;

	[[tooltip("Whether this material is affected by lighting.")]]
    bool lit = true;

    // Lighting data

	[[tooltip("Diffuse texture map.")]]
    Resource<Texture> diffuse_map;

	[[tooltip("Specular texture map.")]]
    Resource<Texture> specular_map;

	[[tooltip("Normal map texture.")]]
    Resource<Texture> normal_map;

	[[tooltip("Shininess value of the material")]]
    float shininess;

	[[tooltip("Whether the material is reflective. Needs a reflection shader to actually work.")]]
	bool reflective;

/*
	[[tooltip("Reflection map of the material. Only used when reflective is enabled.")]]
	Resource<Texture> reflection_map;*/
};
// clang-format on

} // namespace Saturn::Components

#endif
