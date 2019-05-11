#ifndef MVG_VEC_4_HPP_
#define MVG_VEC_4_HPP_

#include "VecBase.hpp"
#include <cassert>
#include <type_traits>

namespace Saturn::Math {

template<typename T>
class Vec<T, 4> {
public:
    static_assert(std::is_floating_point<T>::value,
                  "Saturn::Math: template parameter for Vec4 must be "
                  "floating point type");
    constexpr Vec() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    constexpr Vec(T f) : x(f), y(f), z(f), w(f) {}
    constexpr Vec(T f0, T f1, T f2, T f3) : x(f0), y(f1), z(f2), w(f3) {}
    constexpr Vec(Vec const& rhs) : x(rhs.x), y(rhs.x), z(rhs.z), w(rhs.w) {}

    constexpr Vec& operator=(Vec const& rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    // x/r component of the vector
    union {
        T x;
        T r;
    };
    // y/g component of the vector
    union {
        T y;
        T g;
    };
    // z/b component of the vector
    union {
        T z;
        T b;
    };
    // w/a component of the vector
    union {
        T w;
        T a;
    };

    constexpr T& operator()(std::size_t index) {
        assert(index <= 3);
        if (index == 3) return w;
        if (index == 2) return z;
        return (index == 0) ? x : y;
    }

    constexpr T const& operator()(std::size_t index) const {
        assert(index <= 3);
        if (index == 3) return w;
        if (index == 2) return z;
        return (index == 0) ? x : y;
    }

    constexpr T* ptr() { return &x; }
    constexpr T const* ptr() const { return &x; }
};

template<typename T>
using Vec4 = Vec<T, 4>;

// Basic operator overloads

template<typename T>
constexpr Vec4<T> operator+(Vec4<T> const& lhs, Vec4<T> const& rhs) {
    return Vec4<T>{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

template<typename T>
constexpr Vec4<T> operator-(Vec4<T> const& lhs, Vec4<T> const& rhs) {
    return Vec4<T>{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

template<typename T>
constexpr Vec4<T> operator*(Vec4<T> const& lhs, T scalar) {
    return Vec4<T>{lhs.x * scalar, lhs.y * scalar, lhs.z * scalar,
                   lhs.w * scalar};
}

template<typename T>
constexpr Vec4<T> operator/(Vec4<T> const& lhs, T scalar) {
    return Vec4<T>{lhs.x / scalar, lhs.y / scalar, lhs.z / scalar,
                   lhs.w * scalar};
}

// Comparison operators

template<typename T>
constexpr bool operator==(Vec4<T> const& lhs, Vec4<T> const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

} // namespace Saturn::Math

#endif
