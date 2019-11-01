#ifndef SATURN_MATERIAL_HPP_
#define SATURN_MATERIAL_HPP_

#include "AssetManager/Resource.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Saturn {

class Material {
public:
    Resource<Shader> shader;
    bool lit = true;
    bool reflective = false;
    Resource<Texture> diffuse_map;
    Resource<Texture> specular_map;
    Resource<Texture> normal_map;
    float shininess;

    void swap(Material& other) {
        std::swap(shader, other.shader);
        std::swap(lit, other.lit);
        std::swap(reflective, other.reflective);
        std::swap(diffuse_map, other.diffuse_map);
        std::swap(specular_map, other.specular_map);
        std::swap(normal_map, other.normal_map);
        std::swap(shininess, other.shininess);
    }
};

} // namespace Saturn

#endif // ifndef SATURN_MATERIAL_HPP_
