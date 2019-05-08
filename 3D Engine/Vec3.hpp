#ifndef MVG_VEC_3_HPP_
#define MVG_VEC_3_HPP_

#include "VecBase.hpp"

#include <cassert>
#include <type_traits>

namespace Saturn::Math {

template<typename T>
class Vec<T, 3> {
public:
    static_assert(std::is_floating_point<T>::value,
                  "Saturn::Math: template parameter for Vec3 must be "
                  "floating point type");

    constexpr Vec() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vec(T f) : x(f), y(f), z(f) {}
    constexpr Vec(T f0, T f1, T f2) : x(f0), y(f1), z(f2) {}
    constexpr Vec(Vec const& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

    constexpr Vec& operator=(Vec const& rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
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

    constexpr T& operator()(std::size_t index) {
        assert(index <= 2);
        if (index == 2) return z;
        return (index == 0) ? x : y;
    }

    constexpr T const& operator()(std::size_t index) const {
        assert(index <= 2);
        if (index == 2) return z;
        return (index == 0) ? x : y;
    }

	constexpr T* ptr() { return &x; }
    constexpr T const* ptr() const { return &x; }
};

template<typename T>
using Vec3 = Vec<T, 3>;

// Basic operator overloads

template<typename T>
constexpr Vec3<T> operator+(Vec3<T> const& lhs, Vec3<T> const& rhs) {
    return Vec3<T>{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

template<typename T>
constexpr Vec3<T> operator-(Vec3<T> const& lhs, Vec3<T> const& rhs) {
    return Vec3<T>{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

template<typename T>
constexpr Vec3<T> operator*(Vec3<T> const& lhs, T scalar) {
    return Vec3<T>{lhs.x * scalar, lhs.y * scalar, lhs.z * scalar};
}

template<typename T>
constexpr Vec3<T> operator/(Vec3<T> const& lhs, T scalar) {
    return Vec3<T>{lhs.x / scalar, lhs.y * scalar, lhs.z / scalar};
}

// Comparison operators

template<typename T>
constexpr bool operator==(Vec3<T> const& lhs, Vec3<T> const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

} // namespace Saturn::Math

#endif
