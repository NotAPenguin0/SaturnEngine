#ifndef MVG_VECTOR_MATH_HPP_
#define MVG_VECTOR_MATH_HPP_

#include "Vector.hpp"

namespace Saturn::Math {

// Generated arithmetic operations based on definitions for operator+, -, *, /

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator+=(Vec<T, N>& lhs, Vec<T, N> rhs) {
    lhs = lhs + rhs;
    return lhs;
}

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator-=(Vec<T, N>& lhs, Vec<T, N> rhs) {
    lhs = lhs - rhs;
    return lhs;
}

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator*=(Vec<T, N> lhs, T scalar) {
    lhs = lhs * scalar;
    return lhs;
}

template<typename T, std::size_t N>
constexpr Vec<T, N> operator/=(Vec<T, N> lhs, T scalar) {
    lhs = lhs / scalar;
    return lhs;
}

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator++(Vec<T, N>& lhs) {
    return lhs += Vec<T, N>(1.0);
}

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator++(Vec<T, N>& lhs, int) {
    auto copy = lhs;
    lhs += Vec<T, N>(1.0);
    return copy;
}

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator--(Vec<T, N>& lhs) {
    return lhs -= Vec<T, N>(1.0);
}

template<typename T, std::size_t N>
constexpr Vec<T, N>& operator--(Vec<T, N>& lhs, int) {
    auto copy = lhs;
    lhs -= Vec<T, N>(1.0);
    return copy;
}

// Generated comparison functions generated based on definition of operator==

template<typename T, std::size_t N>
constexpr bool operator!=(Vec<T, N> const& lhs, Vec<T, N> const& rhs) {
    return !(lhs == rhs);
}

// dot and cross product

template<typename T, std::size_t N>
constexpr T dot(Vec<T, N> const& lhs, Vec<T, N> const& rhs) {
    T result = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        result += lhs(i) * rhs(i);
    }
    return result;
}

// Cross product for vector3
template<typename T>
constexpr Vec<T, 3> cross(Vec<T, 3> const& lhs, Vec<T, 3> const& rhs) {
    return Vec<T, 3>{lhs.y * rhs.z - lhs.z * rhs.y,
                     lhs.z * rhs.x - lhs.x * rhs.z,
                     lhs.x * rhs.y - lhs.y * rhs.x};
}

} // namespace Saturn::Math

#endif
