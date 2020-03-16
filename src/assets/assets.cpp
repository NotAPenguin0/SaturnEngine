#include <saturn/assets/assets.hpp>
#include <saturn/assets/importers/simple_mesh.hpp>
#include <saturn/assets/importers/stb_texture_import.hpp>
#include <saturn/assets/importers/obj.hpp>

#include <phobos/renderer/mesh.hpp>
#include <phobos/renderer/texture.hpp>
#include <phobos/renderer/material.hpp>

#include <unordered_map>
#include <stl/assert.hpp>
#include <stl/utility.hpp>

#include <string>

namespace saturn::assets {

namespace {

template<typename T, typename IdType = stl::int64_t>
struct id_generator {
    static inline IdType cur = 0;
    static IdType next() {
        return cur++;
    }
};

template<typename T>
struct AssetData {
    fs::path path;
    T asset;
};

template<typename T>
T* _get_internal(std::unordered_map<stl::int64_t, AssetData<T>>& data, Handle<T> handle) {
    if (handle.id == -1) { return nullptr; }
    auto it = data.find(handle.id);
    if (it == data.end()) {
        return nullptr;
    }

    else return &it->second.asset;
}

template<typename T>
stl::vector<T*> _get_all_internal(std::unordered_map<stl::int64_t, AssetData<T>>& data) {
    stl::vector<T*> assets(stl::tags::uninitialized, data.size());
    for (auto&[id, asset] : data) {
        if (id >= 0) {
            assets[id] = &asset.asset;
        }
    }
    return assets;
}

template<typename T>
fs::path const& _get_path_internal(std::unordered_map<stl::int64_t, AssetData<T>>& data, Handle<T> handle) {
    if (handle.id == -1) { return ""; }

    auto it = data.find(handle.id);
    if (it == data.end()) {
        return "";
    }

    else return it->second.path;
}

template<typename T>
Handle<T> _get_with_path_internal(std::unordered_map<stl::int64_t, AssetData<T>>& data, fs::path const& path) {
    for (auto const&[id, asset] : data) {
        if (asset.path == path) {
            return { id };
        }
    }

    return { -1 };
}

} // anonymous namespace

namespace data {

static std::unordered_map<stl::int64_t, AssetData<ph::Mesh>> meshes;
static std::unordered_map<stl::int64_t, AssetData<ph::Texture>> textures;
static std::unordered_map<stl::int64_t, AssetData<Model>> models;
static std::unordered_map<stl::int64_t, AssetData<ph::Material>> materials;

} // namespace data

Handle<ph::Mesh> take_mesh(ph::Mesh& mesh) {
    stl::int64_t id = id_generator<ph::Mesh>::next();

    data::meshes.emplace(id, AssetData<ph::Mesh>{std::to_string(id), stl::move(mesh)});

    return { id };
}

Handle<ph::Mesh> load_mesh(Context& ctx, fs::path const& path) {
    auto const maybe_already_loaded_handle = _get_with_path_internal(data::meshes, path);
    if (maybe_already_loaded_handle.id != -1) { return maybe_already_loaded_handle; }

    stl::int64_t id = id_generator<ph::Mesh>::next();

    ph::Mesh mesh = importers::import_simple_mesh(ctx, path);
    data::meshes.emplace(id, AssetData<ph::Mesh>{path, stl::move(mesh)});

    ctx.vulkan->logger->write_fmt(ph::log::Severity::Info, "Loaded mesh {}", path.generic_string());

    return { id };
}

ph::Mesh* get_mesh(Handle<ph::Mesh> handle) {
    return _get_internal(data::meshes, handle);
}

fs::path const& get_mesh_path(Handle<ph::Mesh> handle) {
    return _get_path_internal(data::meshes, handle);
}



Handle<ph::Texture> load_texture(Context& ctx, fs::path const& path) {
    // If the path was already loaded at some point, don't load it again
    auto const maybe_already_loaded_handle = _get_with_path_internal(data::textures, path);
    if (maybe_already_loaded_handle.id != -1) { return maybe_already_loaded_handle; }

    stl::int64_t id = id_generator<ph::Texture>::next();

    ph::Texture tex = importers::import_with_stb(ctx, path);
    data::textures.emplace(id, AssetData<ph::Texture>{path, stl::move(tex)});

    ctx.vulkan->logger->write_fmt(ph::log::Severity::Info, "Loaded texture {}", path.generic_string());

    return { id };
}

ph::Texture* get_texture(Handle<ph::Texture> handle) {
    return _get_internal(data::textures, handle);
}

fs::path const& get_texture_path(Handle<ph::Texture> handle) {
    return _get_path_internal(data::textures, handle);
}



Handle<Model> load_model(Context& ctx, fs::path const& path) {
    auto const maybe_already_loaded_handle = _get_with_path_internal(data::models, path);
    if (maybe_already_loaded_handle.id != -1) { return maybe_already_loaded_handle; }

    stl::int64_t id = id_generator<Model>::next();

    Model model = importers::import_obj_model(ctx, path);
    data::models.emplace(id, AssetData<Model>{path, model});

    ctx.vulkan->logger->write_fmt(ph::log::Severity::Info, "Loaded model {}", path.generic_string());

    return { id };
}

Model* get_model(Handle<Model> handle) {
    return _get_internal(data::models, handle);
}

fs::path const& get_model_path(Handle<Model> handle) {
    return _get_path_internal(data::models, handle);
}

Handle<ph::Material> take_material(ph::Material& material) {
    stl::int64_t id = id_generator<ph::Mesh>::next();

    data::materials.emplace(id, AssetData<ph::Material>{"", stl::move(material)});

    return { id };
}

Handle<ph::Material> load_material(Context& ctx, fs::path const& path) {
    auto const maybe_already_loaded_handle = _get_with_path_internal(data::materials, path);
    if (maybe_already_loaded_handle.id != -1) { return maybe_already_loaded_handle; }

    stl::int64_t id = id_generator<Model>::next();

    // TODO: No mtl importer yet

    return { -1 };
}

ph::Material* get_material(Handle<ph::Material> handle) {
    return _get_internal(data::materials, handle);
}

fs::path const& get_material_path(Handle<ph::Material> handle) {
    return _get_path_internal(data::materials, handle);
}

stl::vector<ph::Material*> get_all_materials() {
    return _get_all_internal(data::materials);
}

void destroy_all_assets() {
    for (auto&[id, mesh] : data::meshes) {
        mesh.asset.destroy();
    }

    for (auto&[id, texture] : data::textures) {
        texture.asset.destroy();
    }
}

}