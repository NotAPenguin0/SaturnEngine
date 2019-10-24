#ifndef SATURN_MATERIAL_HPP_
#define SATURN_MATERIAL_HPP_

#include "AssetManager/Resource.hpp"
#include "ComponentBase.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] Material
    : public ComponentBase {

	[[saturn::tooltip("The shader this material will use.")]]
    Resource<Shader> shader;

	[[saturn::tooltip("Whether this material is affected by lighting.")]]
    bool lit = true;

    // Lighting data

	[[saturn::tooltip("Diffuse texture map.")]]
    Resource<Texture> diffuse_map;

	[[saturn::tooltip("Specular texture map.")]]
    Resource<Texture> specular_map;

	[[saturn::tooltip("Normal map texture.")]]
    Resource<Texture> normal_map;

	[[saturn::tooltip("Shininess value of the material")]]
    float shininess;
};
// clang-format on

} // namespace Saturn::Components

#endif
