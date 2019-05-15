#ifndef MVG_ASSET_MANAGER_HPP_
#define MVG_ASSET_MANAGER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "Resource.hpp"
#include "ResourceLoaders.hpp"
#include "Utility/IDGenerator.hpp"

namespace Saturn {

template<typename R>
class AssetManager {
public:
    // If the resource is not loaded yet, this function will load it. Otherwise
    // it just returns the resource
    static Resource<R> get_resource(std::string const& path) {
        // Check if the resource has been loaded already
        if (id_map.find(path) != id_map.end()) {
            auto id = id_map[path];
            R* raw = resources[id].get();
            return Resource<R>(raw, id, true);
        }

        auto id = IDGenerator<R>::next();
        std::unique_ptr<R> res = ResourceLoader<R>::load(path);
        if (res == nullptr) {
            return Resource<R>(nullptr, -1, false);
        } else {
            R* raw = res.get();
            resources[id] = std::move(res);
            return Resource<R>(raw, id, true);
        }
    }

    // Resource must be loaded already.
    static Resource<R> get_loaded_resource(std::size_t id) {
        return resources.at(id);
    }

	//#TODO: Smart resource unloading (refcount in Resource<R>)

    static void unload(std::size_t id) { resources.erase(resources.find(id)); }

private:
    static inline std::unordered_map<std::size_t, std::unique_ptr<R>> resources;
    static inline std::unordered_map<std::string, std::size_t> id_map;
};

} // namespace Saturn

#endif
