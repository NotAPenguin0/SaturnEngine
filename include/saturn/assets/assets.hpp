#ifndef SATURN_ASSETS_HPP_
#define SATURN_ASSETS_HPP_

#include <saturn/utility/handle.hpp>
#include <saturn/utility/context.hpp>

#include <phobos/forward.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace saturn::assets {

Handle<ph::Mesh> load_mesh(Context& ctx, fs::path const& path);
ph::Mesh* get_mesh(Handle<ph::Mesh> handle);
fs::path const& get_mesh_path(Handle<ph::Mesh> handle);

Handle<ph::Texture> load_texture(Context& ctx, fs::path const& path);
ph::Texture* get_texture(Handle<ph::Texture> handle);
fs::path const& get_texture_path(Handle<ph::Texture> handle);

void destroy_all_assets();

}

#endif