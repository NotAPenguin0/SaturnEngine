#ifndef SATURN_RESOURCE_LOADERS_HPP_
#define SATURN_RESOURCE_LOADERS_HPP_

#include <memory>
#include <string>

#include "Subsystems/Renderer/Mesh.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/Texture.hpp"

#include <audeo/audeo.hpp>

namespace Saturn {

template<typename R>
class ResourceLoader {
public:
    static std::unique_ptr<R> load(std::string const& path, std::string const& root_dir) {
        static_assert("No resource loader found for specified type.");
    }
};

template<>
class ResourceLoader<Shader> {
public:
    static std::unique_ptr<Shader> load(std::string const& path, std::string const& root_dir);
};

template<>
class ResourceLoader<Mesh> {
public:
    static std::unique_ptr<Mesh> load(std::string const& path, std::string const& root_dir);
};

template<>
class ResourceLoader<Texture> {
public:
    static std::unique_ptr<Texture> load(std::string const& path, std::string const& root_dir);
};

template<>
class ResourceLoader<audeo::SoundSource> {
public:
    static std::unique_ptr<audeo::SoundSource> load(std::string const& path, std::string const& root_dir);
};

// File types

template<typename R>
struct FileTypes;

struct FileType {
    const wchar_t* name;
	const wchar_t* filter;
};

template<>
struct FileTypes<Shader> {
    static std::vector<FileType> types;
};

template<>
struct FileTypes<Mesh> {
    static std::vector<FileType> types;
};

template<>
struct FileTypes<Texture> {
    static std::vector<FileType> types;
};

template<>
struct FileTypes<audeo::SoundSource> {
    static std::vector<FileType> types;
};

} // namespace Saturn

#endif
