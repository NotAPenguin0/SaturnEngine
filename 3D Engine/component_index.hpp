#ifndef MVG_COMPONENT_INDEX_HPP_
#define MVG_COMPONENT_INDEX_HPP_

#include <cstddef>
#include <type_traits>

namespace Saturn {

namespace detail {

template<std::size_t I, typename T, typename... Ts>
struct index_of_impl;

template<std::size_t I, typename T, typename Head, typename... Tail>
struct index_of_impl<I, T, Head, Tail...> {
    static constexpr std::size_t value = std::is_same<T, Head>::value
                                   ? I
                                   : index_of_impl<I + 1, T, Tail...>::value;
};

// Fallback if not found or last element to end recursion
template<std::size_t I, typename T, typename Head>
struct index_of_impl<I, T, Head> {
    static constexpr std::size_t value =
        std::is_same<T, Head>::value ? I : static_cast<std::size_t>(-1);
};

template<typename T, typename... Ts>
struct index_of {
    static constexpr std::size_t value = index_of_impl<0, T, Ts...>::value;
};

} // namespace detail

template<typename... Cs>
struct component_index_table {
    template<typename C>
    std::size_t get() const {
        return detail::index_of<C, Cs...>::value;
    }
	
    const std::size_t not_found = static_cast<std::size_t>(-1);
};

} // namespace Saturn

#endif
