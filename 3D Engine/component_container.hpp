#ifndef MVG_COMPONENT_HPP_
#define MVG_COMPONENT_HPP_

#include "type_erased.hpp"

#include <typeinfo>
#include <vector>

namespace Saturn {

namespace detail {

class component_container_interface {
public:
    // #TODO: add interface here

    virtual std::type_info get_component_type() const = 0;
};

} // namespace detail

template<typename C>
class component_container : public detail::component_container_interface {
public:
    // #TODO: add implementation here

    virtual std::type_info get_component_type() const override {
        return typeid(C);
    }

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
