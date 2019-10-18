#ifndef SATURN_ASSET_MANAGER_HPP_
#define SATURN_ASSET_MANAGER_HPP_

#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

namespace fs = std::filesystem;

#include "AssetTracker.hpp"
#include "Resource.hpp"
#include "ResourceLoaders.hpp"
#include "Utility/IDGenerator.hpp"

#include "Editor/ProjectFile.hpp"

namespace Saturn {

template<typename R>
class AssetManager {
public:
    static void init() { asset_tracker = std::make_unique<AssetTracker<R>>(); }

    struct Asset {
        std::unique_ptr<R> ptr;
        fs::path path;
        bool imported;
        size_t id;
        std::vector<fs::path> dependent_paths;
    };

    // If the resource is not loaded yet, this function will load it.
    // Otherwise it just returns the resource
    static Resource<R> get_resource(std::string const& path,
                                    bool use_engine_path = false,
                                    bool imported = false) {
        std::string new_p = path;
        if (!use_engine_path) {
            new_p = (Editor::ProjectFile::root_path() / path).generic_string();
        }
        // Check if the resource has been loaded already
        if (id_map.find(fs::absolute(fs::path(new_p)).generic_string()) !=
            id_map.end()) {
            auto id = id_map[fs::absolute(fs::path(new_p)).generic_string()];
            R* raw = resources[id].ptr.get();
            return Resource<R>(raw, id, true,
                               fs::absolute(fs::path(new_p)).generic_string());
        }

        auto id = IDGenerator<R>::next();
        auto res = ResourceLoader<R>::load(
            new_p,
            use_engine_path ? "" : Editor::ProjectFile::root_path().string());
        if (res.ptr == nullptr) {
            return Resource<R>(nullptr, -1, false, new_p);
        } else {
            id_map[fs::absolute(fs::path(new_p)).generic_string()] = id;
            R* raw = res.ptr.get();
            resources[id].ptr = std::move(res.ptr);
            resources[id].path = fs::absolute(fs::path(new_p));
            resources[id].imported = imported;
            resources[id].id = id;
            resources[id].dependent_paths = std::move(res.dependent_paths);
            if (imported) {
                std::vector<fs::path> associated_paths =
                    resources[id].dependent_paths;
                associated_paths.push_back(resources[id].path);
                asset_tracker->start_tracking(resources[id],
                                              std::move(associated_paths));
            }
            return Resource<R>(raw, id, true,
                               fs::absolute(fs::path(new_p)).generic_string());
        }
    }

    template<typename CreateInfoT>
    static Resource<R> get_resource(CreateInfoT const& info,
                                    std::string const& name) {
        static_assert(std::is_same_v<CreateInfoT, typename R::CreateInfo>,
                      "Must be a CreateInfo type");
        auto id = IDGenerator<R>::next();
        std::unique_ptr<R> res = std::make_unique<R>(info);
        if (res == nullptr) {
            return Resource<R>(nullptr, -1, false, "");
        } else {
            id_map[fs::absolute(fs::path(name)).generic_string()] = id;
            R* raw = res.get();
            resources[id].ptr = std::move(res);
            resources[id].path = fs::absolute(fs::path(name));
            resources[id].imported = false;
            resources[id].id = id;
            return Resource<R>(raw, id, true,
                               fs::absolute(fs::path(name)).generic_string());
        }
    }

    // Resource must be loaded already.
    static Resource<R> get_loaded_resource(std::size_t id) {
        auto& res = resources.at(id);
        return Resource<R>(res.ptr.get(), id, true, res.path.generic_string());
    }

    static void unload(std::size_t id) { resources.erase(resources.find(id)); }

    static std::unordered_map<size_t, Asset>& resource_list() {
        return resources;
    }

    static void remove_asset(Asset& asset) {
        asset_tracker->stop_tracking(asset);
        id_map.erase(asset.path.generic_string());
        resources.erase(asset.id);
    }

    static void queue_import(fs::path path) { import_list.push(path); }

    static void do_imports() {
        while (!import_list.empty()) {
            auto const& path = import_list.front();
            // load the resource, but discard the handle. This should never use
            // the engine path since engine assets will not be imported this way
            auto imported =
                AssetManager<R>::get_resource(path.string(), false, true);
            import_list.pop();
        }
    }

    static void queue_reload(Asset* asset) { reload_queue.push(asset); }

    static void do_reloads() {
        while (!reload_queue.empty()) {
            Asset* asset = reload_queue.front();
            ResourceLoader<R>::reload(
                asset->ptr, asset->dependent_paths,
                asset->path.generic_string(),
                Editor::ProjectFile::root_path().generic_string());
            reload_queue.pop();
        }
    }

private:
    static inline std::unordered_map<std::size_t, Asset> resources;
    static inline std::unordered_map<std::string, std::size_t> id_map;

    static inline std::queue<fs::path> import_list;
    static inline std::queue<Asset*> reload_queue;

    static inline std::unique_ptr<AssetTracker<R>> asset_tracker = nullptr;
};

template<typename R>
void from_json(nlohmann::json const& j, Resource<R>& res) {
    if (auto r = j.find("Resource"); r != j.end()) {
        auto path = j["Resource"];
        res = AssetManager<R>::get_resource(path);
    }
}

template<typename R>
void to_json(nlohmann::json& j, Resource<R> const& res) {
    j["Resource"] =
        fs::relative(res.get_path(), Editor::ProjectFile::root_path()).string();
}

template<typename A>
std::string_view asset_type_string() {
    if constexpr (std::is_same_v<A, Shader>) { return "shader"; }
    if constexpr (std::is_same_v<A, Texture>) { return "texture"; }
    if constexpr (std::is_same_v<A, Mesh>) { return "mesh"; }
    if constexpr (std::is_same_v<A, audeo::SoundSource>) { return "sound"; }
    if constexpr (std::is_same_v<A, Font>) { return "font"; }
}

} // namespace Saturn

#endif
