#ifndef SATURN_RESOURCE_LOADERS_HPP_
#define SATURN_RESOURCE_LOADERS_HPP_

#include <memory>
#include <string>
#include <filesystem>

#include "Renderer/Mesh.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

#include <audeo/audeo.hpp>

namespace fs = std::filesystem;

namespace Saturn {
template<typename R>
struct LoadResult {
    std::unique_ptr<R> ptr;
    std::vector<fs::path> dependent_paths;
};

template<typename R>
class ResourceLoader {
public:
    static LoadResult<R> load(std::string const& path,
                                   std::string const& root_dir) {
        static_assert("No resource loader found for specified type.");
    }

    static void reload(std::unique_ptr<R>& res, std::vector<fs::path>& dep_paths,
                       std::string const& path,
                       std::string const& root_dir) {
        static_assert("No resource loader found for specified type.");
    }
};

template<>
class ResourceLoader<Shader> {
public:
    static LoadResult<Shader> load(std::string const& path,
                                        std::string const& root_dir);
    static void reload(std::unique_ptr<Shader>& res,
                       std::vector<fs::path>& dep_paths,
                       std::string const& path,
                       std::string const& root_dir);
};

template<>
class ResourceLoader<Mesh> {
public:
    static LoadResult<Mesh> load(std::string const& path,
                                      std::string const& root_dir);
    static void reload(std::unique_ptr<Mesh>& res,
                       std::vector<fs::path>& dep_paths,
                       std::string const& path,
                       std::string const& root_dir);
};

template<>
class ResourceLoader<Texture> {
public:
    static LoadResult<Texture> load(std::string const& path,
                                         std::string const& root_dir);
    static void reload(std::unique_ptr<Texture>& res,
                       std::vector<fs::path>& dep_paths,
                       std::string const& path,
                       std::string const& root_dir);
};

template<>
class ResourceLoader<audeo::SoundSource> {
public:
    static LoadResult<audeo::SoundSource>
    load(std::string const& path, std::string const& root_dir);
    static void reload(std::unique_ptr<audeo::SoundSource>& res,
                       std::vector<fs::path>& dep_paths,
                       std::string const& path,
                       std::string const& root_dir);
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
