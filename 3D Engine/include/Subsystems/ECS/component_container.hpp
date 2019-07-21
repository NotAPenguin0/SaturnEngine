#ifndef MVG_COMPONENT_CONTAINER_HPP_
#define MVG_COMPONENT_CONTAINER_HPP_

#include "Utility/IDGenerator.hpp"
#include "Utility/type_erased.hpp"

#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace Saturn {

namespace detail {

class component_container_interface {
public:
    virtual ~component_container_interface() = 0;

    virtual std::type_info const& get_component_type() const = 0;
};

} // namespace detail

template<typename C>
class component_container : public detail::component_container_interface {
public:
    using value_type = C;
    using reference = C&;
    using const_reference = C const&;
    using pointer = C*;
    using iterator = typename std::vector<C>::iterator;
    using const_iterator = typename std::vector<C>::const_iterator;

    component_container() = default;
    component_container(component_container const&) = default;
    component_container(component_container&&) = default;

    component_container& operator=(component_container const&) = default;
    component_container& operator=(component_container&&) = default;
    ~component_container() override = default;

    virtual std::type_info const& get_component_type() const override {
        return typeid(C);
    }

    iterator push_back(C const& c) {
        components.push_back(c);
        auto id = IDGenerator<C>::next();
        components.back().id = id;                // Assign correct id
        id_index_map[id] = components.size() - 1; // Update index map
        return components.end() - 1;
    }

    iterator erase_component(std::size_t id) {
        // Algorithm for erasing and updating indices:
        /*
        1. Swap element to erase with last element
        2. Update index for swapped elements
        3. Erase
        */

        auto id_to_update = components.back().id;
        auto erased_idx = id_index_map.at(id);
        std::swap(components[erased_idx], components.back());
        id_index_map[id_to_update] = erased_idx;
        return components.erase(components.end() - 1);
    }

    reference get_with_id(std::size_t id) {
        return (*this)[id_index_map.at(id)];
    }

    const_reference get_with_id(std::size_t id) const {
        return (*this)[id_index_map.at(id)];
    }

    std::size_t size() const { return components.size(); }
    bool empty() const { return components.empty(); }

    iterator begin() { return components.begin(); }
    const_iterator begin() const { return components.begin(); }
    const_iterator cbegin() const { return components.cbegin(); }

    iterator end() { return components.end(); }
    const_iterator end() const { return components.end(); }
    const_iterator cend() const { return components.cend(); }

private:
    reference operator[](std::size_t index) { return components[index]; }

    const_reference operator[](std::size_t index) const {
        return components[index];
    }

    reference at(std::size_t index) { return components.at(index); }
    const_reference at(std::size_t index) const { return components.at(index); }

    std::vector<C> components;
    std::unordered_map<std::size_t, std::size_t> id_index_map;
};

// Scene will store a vector<mvg::type_erased<container_iterface,
// component_container>>

using any_component_container =
    mvg::type_erased<detail::component_container_interface,
                     component_container>;

} // namespace Saturn

#endif
