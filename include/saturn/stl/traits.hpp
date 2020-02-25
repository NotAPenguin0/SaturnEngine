#ifndef SATURN_STL_TRAITS_HPP_
#define SATURN_STL_TRAITS_HPP_

#include <saturn/stl/types.hpp>

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


} // namespace stl

#endif