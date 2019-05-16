#ifndef MVG_ECS_HPP_
#define MVG_ECS_HPP_

#include <tuple>
#include <vector>

#include "component_container.hpp"
#include "component_index.hpp"
#include "component_view.hpp"

#include "Subsystems/Scene/SceneObject.hpp"

#include "Systems/SystemBase.hpp"

namespace Saturn {

class Scene;

template<typename... Cs>
class ECS {
public:
    ECS(Scene* s) : scene(s) { create_component_containers<Cs...>(); }
    ECS(ECS const&) = delete;
    ECS(ECS&&) = delete;

    ECS& operator=(ECS const&) = delete;
    ECS& operator=(ECS&&) = delete;

    void on_start() {
        for (auto& system : systems) { system->on_start(*scene); }
    }

    template<typename S>
    void register_system() {
        systems.push_back(std::make_unique<S>());
    }

    template<typename Head, typename... Tail>
    void register_systems() {
        register_system<Head>();
        if constexpr (sizeof...(Tail) != 0) register_systems<Tail...>();
    }

    void update_systems() {
        for (auto& system : systems) { system->on_update(*scene); }
    }

    // Assumes that ptr is a valid pointer returned from
    // find_component_container. C is the COMPONENT TYPE
    template<typename C>
    component_container<C>& get_components() {
        auto* any_container = find_component_container<C>();
        assert(any_container != nullptr);
        return *((*any_container).template get_as<component_container<C>>());
    }

    // Grabs all component sets with a specified set of components
    template<typename... Cs>
    component_view<Cs...> select() {
        return component_view<Cs...>(scene->objects.begin(),
                                     scene->objects.end());
    }

    template<typename C>
    C& get_with_id(std::size_t id) {
        auto& container = get_components<C>();
        return container.get_with_id(id);
    }

    template<typename C>
    any_component_container* find_component_container() {
        auto idx = component_indices.template get<C>();
        if (idx == component_indices.not_found) return nullptr;
        return &(components[idx]);
    }

    template<typename C>
    any_component_container const* find_component_container() const {
        // Warning: Has to be the same implementation as non-const version!
        auto idx = component_indices.get<C>();
        if (idx == component_indices.not_found) return nullptr;
        return &(components[idx]);
    }

    template<typename C>
    void add_component_container() {
        // Insert new element and put a default constructed container for
        // component C in it
        components.emplace_back().template emplace<component_container<C>>();

        // components.back().emplace<component_container<C>>();
    }

private:
    template<typename Head, typename... Tail>
    void create_component_containers() {
        add_component_container<Head>();
        if constexpr (sizeof...(Tail) != 0)
            create_component_containers<Tail...>();
    }

    std::vector<any_component_container> components;
    std::vector<std::unique_ptr<Systems::SystemBase>> systems;
    component_index_table<Cs...> component_indices;
    Scene* scene;
};

} // namespace Saturn

#endif
