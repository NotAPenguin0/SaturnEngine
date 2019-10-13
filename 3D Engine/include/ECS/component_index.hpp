#ifndef SATURN_COMPONENT_INDEX_HPP_
#define SATURN_COMPONENT_INDEX_HPP_

#include <cstddef>
#include <type_traits>

#include "Utility/Utility.hpp"

namespace Saturn {

template<typename... Cs>
struct component_index_table {
    template<typename C>
    std::size_t get() const {
        return index_of<C, Cs...>::value;
    }

    const std::size_t not_found = static_cast<std::size_t>(-1);
};

} // namespace Saturn

#endif
