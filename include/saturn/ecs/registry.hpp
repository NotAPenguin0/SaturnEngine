#ifndef SATURN_ECS_REGISTRY_HPP_
#define SATURN_ECS_REGISTRY_HPP_

#include <saturn/ecs/component_storage.hpp>
#include <saturn/ecs/component_id.hpp>
#include <saturn/ecs/component_view.hpp>

#include <saturn/stl/vector.hpp>
#include <saturn/stl/unique_ptr.hpp>

namespace saturn::ecs {

class registry {
public:
    registry() = default;
    registry(registry const&) = delete;
    registry(registry&&) = default;
    
    registry& operator=(registry const&) = delete;
    registry& operator=(registry&&) = default;

    ~registry() = default;

    entity_t create_entity();

    template<typename T, typename... Args>
    void add_component(entity_t entity, Args&&... args) {
        component_storage<T>& storage = get_or_emplace_storage<T>();
        storage.construct(entity, std::forward<Args>(args) ...);
    }

    template<typename T>
    bool has_component(entity_t entity) {
        component_storage<T>& storage = get_or_emplace_storage<T>();
        auto const it = storage.find(entity);
        return it != storage.end();
    }

    template<typename T>
    T& get_component(entity_t entity) {
        component_storage<T>& storage = get_or_emplace_storage<T>();
        return *storage.find(entity);
    }
    

    template<typename... Ts>
    component_view<Ts...> view() {
        return { get_or_emplace_storage<Ts>() ... };
    }

private:
    struct storage_data {
        stl::uint64_t type_id;
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
        stl::uint64_t const index = component_type_id<T>::id;

        // If the index is not found, we have to register the new component
        if (index >= storages.size()) {
            storages.emplace_back();
            storages.back().type_id = index;    
            storages.back().storage = std::make_unique<component_storage<T>>();
        }

        storage_data& storage = storages[index];
        return *static_cast<component_storage<T>*>(storage.storage.get());
    }

    stl::vector<entity_t> entities;
    stl::vector<storage_data> storages;
};

} // namespace saturn::ecs

#endif