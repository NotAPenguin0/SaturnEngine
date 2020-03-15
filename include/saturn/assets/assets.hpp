#ifndef SATURN_ASSETS_HPP_
#define SATURN_ASSETS_HPP_

#include <saturn/utility/handle.hpp>
#include <saturn/utility/context.hpp>

#include <saturn/assets/model.hpp>

#include <phobos/forward.hpp>

#include <stl/vector.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace saturn::assets {

// MESH

// Takes ownership of given mesh and returns a handle to it
Handle<ph::Mesh> take_mesh(ph::Mesh& mesh);

Handle<ph::Mesh> load_mesh(Context& ctx, fs::path const& path);

ph::Mesh* get_mesh(Handle<ph::Mesh> handle);

fs::path const& get_mesh_path(Handle<ph::Mesh> handle);

// TEXTURE

Handle<ph::Texture> load_texture(Context& ctx, fs::path const& path);

ph::Texture* get_texture(Handle<ph::Texture> handle);

fs::path const& get_texture_path(Handle<ph::Texture> handle);

// MODEL

Handle<Model> load_model(Context& ctx, fs::path const& path);

Model* get_model(Handle<Model> handle);

fs::path const& get_model_path(Handle<Model> handle);

// MATERIAL

Handle<ph::Material> take_material(ph::Material& material);

Handle<ph::Material> load_material(Context& ctx, fs::path const& path);

ph::Material* get_material(Handle<ph::Material> handle);

fs::path const& get_material_path(Handle<ph::Material> handle);

stl::vector<ph::Material*> get_all_materials();

void destroy_all_assets();

}

#endif