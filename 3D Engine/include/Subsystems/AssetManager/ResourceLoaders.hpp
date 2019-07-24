#ifndef SATURN_RESOURCE_LOADERS_HPP_
#define SATURN_RESOURCE_LOADERS_HPP_

#include <memory>
#include <string>

#include "Subsystems/Renderer/Mesh.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/Texture.hpp"

namespace Saturn {

template<typename R>
class ResourceLoader {
public:
    static std::unique_ptr<R> load(std::string const& path) {
        static_assert("No resource loader found for specified type.");
    }
};

template<>
class ResourceLoader<Shader> {
public:
    static std::unique_ptr<Shader> load(std::string const& path);
};

template<>
class ResourceLoader<Mesh> {
public:
    static std::unique_ptr<Mesh> load(std::string const& path);
};

template<>
class ResourceLoader<Texture> {
public:
    static std::unique_ptr<Texture> load(std::string const& path);
};

} // namespace Saturn

#endif
