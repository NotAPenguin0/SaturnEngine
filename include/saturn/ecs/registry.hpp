#ifndef SATURN_ECS_REGISTRY_HPP_
#define SATURN_ECS_REGISTRY_HPP_

#include <saturn/ecs/component_storage.hpp>
#include <saturn/ecs/component_id.hpp>
#include <saturn/ecs/component_view.hpp>

#include <stl/vector.hpp>
#include <stl/tree.hpp>
#include <stl/unique_ptr.hpp>

namespace saturn::ecs {

class registry {
public:
    registry();
    registry(registry const&) = delete;
    registry(registry&&) = default;
    
    registry& operator=(registry const&) = delete;
    registry& operator=(registry&&) = default;

    ~registry() = default;

    entity_t create_entity(entity_t parent = 0);
    entity_t create_blueprint_entity(entity_t parent = 0);

    // 'Imports' an entity from registry [source] to this registry. Effectively makes a copy of all entity data
    entity_t import_blueprint(registry& source, entity_t other);

    template<typename T, typename... Args>
    void add_component(entity_t entity, Args&&... args) {
        component_storage<T>& storage = get_or_emplace_storage<T>();
        storage.construct(entity, std::forward<Args>(args) ...);
    }

    template<typename T>
    bool has_component(entity_t entity) const {
        component_storage<T> const& storage = get_or_emplace_storage<T>();
        auto const it = storage.find(entity);
        return it != storage.end();
    }

    template<typename T>
    T& get_component(entity_t entity) {
        component_storage<T>& storage = get_or_emplace_storage<T>();
        return *storage.find(entity);
    }

    template<typename T>
    T const& get_component(entity_t entity) const {
        component_storage<T> const& storage = get_or_emplace_storage<T>();
        return *storage.find(entity);
    }
    

    template<typename... Ts>
    component_view<Ts...> view() {
        return { get_or_emplace_storage<Ts>() ... };
    }

    stl::tree<entity_t> const& get_entities() const;

private:
    struct storage_data {
        stl::uint64_t type_id = 0;
        stl::unique_ptr<component_storage_base> storage;
    };

    struct entity_id_generator {
        entity_t cur = 0;

        entity_t next() {
            return cur++;
        }
    } id_generator;

    template<typename T>
    component_storage<T>& get_or_emplace_storage() {
        stl::uint64_t const index = get_component_type_id<T>();

        // If the index is not found, we have to register the new component
        if (index >= storages.size()) {
            storages.resize(index + 1);
            storages[index].type_id = index;    
            storages[index].storage = stl::make_unique<component_storage<T>>();
        }
        // Initialize storage if it wasn't created yet
        if (storages[index].storage == nullptr) {
            storages[index].type_id = index;
            storages[index].storage = stl::make_unique<component_storage<T>>();
        }

        storage_data& storage = storages[index];
        return *static_cast<component_storage<T>*>(storage.storage.get());
    }

    template<typename T>
    component_storage<T> const& get_or_emplace_storage() const {
        stl::uint64_t const index = get_component_type_id<T>();

        // If the index is not found, we have to register the new component
        if (index >= storages.size()) {
            storages.resize(index + 1);
            storages[index].type_id = index;    
            storages[index].storage = stl::make_unique<component_storage<T>>();
        }
        // Initialize storage if it wasn't created yet
        if (storages[index].storage == nullptr) {
            storages[index].type_id = index;
            storages[index].storage = stl::make_unique<component_storage<T>>();
        }

        storage_data const& storage = storages[index];
        return *static_cast<component_storage<T> const*>(storage.storage.get());
    }

    stl::tree<entity_t> entities;
    mutable stl::vector<storage_data> storages;
};

} // namespace saturn::ecs

#endif