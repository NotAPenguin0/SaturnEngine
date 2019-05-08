#ifndef MVG_ECS_HPP_
#define MVG_ECS_HPP_

#include <vector>

#include "component_container.hpp"
#include "component_index.hpp"

namespace Saturn {

template<typename... Cs>
class ECS {
public:
    ECS() { create_component_containers<Cs...>(); }
    ECS(ECS const&) = delete;
    ECS(ECS&&) = delete;

    ECS& operator=(ECS const&) = delete;
    ECS& operator=(ECS&&) = delete;

    // Assumes that ptr is a valid pointer returned from
    // find_component_container. C is the COMPONENT TYPE
    template<typename C>
    component_container<C>& get_components() {
        auto* any_container = find_component_container<C>();
        assert(any_container != nullptr);
        return *((*any_container).template get_as<component_container<C>>());
    }

    template<typename... Cs>
    void /*Figure out a container*/ select() {
        // #TODO: Function to grab all entities with a set of components
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
    component_index_table<Cs...> component_indices;
};

} // namespace Saturn

#endif
