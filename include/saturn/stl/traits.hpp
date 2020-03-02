#ifndef SATURN_STL_TRAITS_HPP_
#define SATURN_STL_TRAITS_HPP_

#include <saturn/stl/types.hpp>

#include <type_traits>

namespace stl {

struct true_type {
    static constexpr bool value = true;
};

struct false_type {
    static constexpr bool value = false;
};

template<typename T>
struct is_integral : false_type {};

template<>
struct is_integral<int8_t> : true_type {};
template<>
struct is_integral<int16_t> : true_type {};
template<>
struct is_integral<int32_t> : true_type {};
template<>
struct is_integral<int64_t> : true_type {};
template<>
struct is_integral<uint8_t> : true_type {};
template<>
struct is_integral<uint16_t> : true_type {};
template<>
struct is_integral<uint32_t> : true_type {};
template<>
struct is_integral<uint64_t> : true_type {};

template<typename T>
constexpr bool is_integral_v = is_integral<T>::value;

template<typename T>
struct is_unsigned : false_type {};

template<>
struct is_unsigned<uint8_t> : true_type {};
template<>
struct is_unsigned<uint16_t> : true_type {};
template<>
struct is_unsigned<uint32_t> : true_type {};
template<>
struct is_unsigned<uint64_t> : true_type {};

template<typename T>
constexpr bool is_unsigned_v = is_unsigned<T>::value;

namespace detail {

template<typename T, template<typename, typename> typename F, typename... Options>
struct find_best_match_impl;

template<typename T, template<typename, typename> typename F, typename Cur>
struct find_best_match_impl<T, F, Cur> {
    using type = Cur;
};

template<typename T, template<typename, typename> typename F, typename Cur, typename Head, typename... Next> 
struct find_best_match_impl<T, F, Cur, Head, Next...> {
    using type = std::conditional_t<F<T, Head>::value, 
        typename find_best_match_impl<T, F, Head, Next...>::type,
        typename find_best_match_impl<T, F, Cur, Next...>::type>;
};

template<typename E, typename T>
struct underlying_enum_type_filter {
    static constexpr bool value = sizeof(E) == sizeof(T) && (T(-1) < T(0) == E(-1) < E(0));
};

} // namespace detail

template<typename E>
struct underlying_type {
    using type = typename detail::find_best_match_impl<E, detail::underlying_enum_type_filter, 
        uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t
    >::type;
};

template<typename E>
using underlying_type_t = typename underlying_type<E>::type;

} // namespace stl

#endif