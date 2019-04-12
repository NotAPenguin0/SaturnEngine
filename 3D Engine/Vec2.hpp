#ifndef MVG_VEC_2_HPP_
#define MVG_VEC_2_HPP_

#include "VecBase.hpp"

#include <cassert>
#include <type_traits>

namespace Saturn::Math {

template<typename T>
class Vec<T, 2> {
public:
    static_assert(std::is_floating_point<T>::value,
                  "Saturn::Math: template parameter for Vec2 must be floating "
                  "point type");

    constexpr Vec() : x(0.0), y(0.0) {}
    constexpr Vec(T f) : x(f), y(f) {}
    constexpr Vec(T f0, T f1) : x(f0), y(f1) {}
    constexpr Vec(Vec const& rhs) : x(rhs.x), y(rhs.y) {}

    constexpr Vec& operator=(Vec const& rhs) {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    // x component of the vector
    T x;
    // y component of the vector
    T y;

    constexpr T& operator()(std::size_t index) {
        assert(index <= 1);
        return (index == 0) ? x : y;
    }

    constexpr T const& operator()(std::size_t index) const {
        assert(index <= 1);
        return (index == 0) ? x : y;
    }
};

template<typename T>
using Vec2 = Vec<T, 2>;

// Basic operator overloads

template<typename T>
constexpr Vec2<T> operator+(Vec2<T> const& lhs, Vec2<T> const& rhs) {
    return Vec2<T>{lhs.x + rhs.x, lhs.y + rhs.y};
}

template<typename T>
constexpr Vec2<T> operator-(Vec2<T> const& lhs, Vec2<T> const& rhs) {
    return Vec2<T>{lhs.x - rhs.x, lhs.y - rhs.y};
}

template<typename T>
constexpr Vec2<T> operator*(Vec2<T> const& lhs, T scalar) {
    return Vec2<T>{lhs.x * scalar, lhs.y * scalar};
}

template<typename T>
constexpr Vec2<T> operator/(Vec2<T> const& lhs, T scalar) {
    return Vec2<T>{lhs.x / scalar, lhs.y * scalar};
}

// Comparison operators

template<typename T>
constexpr bool operator==(Vec2<T> const& lhs, Vec2<T> const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

} // namespace Saturn::Math

#endif
