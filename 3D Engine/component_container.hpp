#ifndef MVG_COMPONENT_CONTAINER_HPP_
#define MVG_COMPONENT_CONTAINER_HPP_

#include "type_erased.hpp"

#include <typeinfo>
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
    ~component_container() override {
        if (components.empty()) {
            int x = 0;
            x = x + 1;
        }
    }

    // #TODO: Add functionality here (nonvirtual, use cast to access)

    virtual std::type_info const& get_component_type() const override {
        return typeid(C);
    }

    iterator push_back(C const& c) {
        components.push_back(c);
        return components.end() - 1;
    }

    iterator erase_component(iterator it) { return components.erase(it); }

    std::size_t size() const { return components.size(); }
    bool empty() const { return components.empty(); }

    reference operator[](std::size_t index) { return components[index]; }

    const_reference operator[](std::size_t index) const {
        return components[index];
    }

    reference at(std::size_t index) { return components.at(index); }
    const_reference at(std::size_t index) const { return components.at(index); }

    iterator begin() { return components.begin(); }
    const_iterator begin() const { return components.begin(); }
    const_iterator cbegin() const { return components.cbegin(); }

    iterator end() { return components.end(); }
    const_iterator end() const { return components.end(); }
    const_iterator cend() const { return components.cend(); }

private:
    std::vector<C> components;
};

// Scene will store a vector<mvg::type_erased<container_iterface,
// component_container>>

using any_component_container =
    mvg::type_erased<detail::component_container_interface,
                     component_container>;

} // namespace Saturn

#endif
