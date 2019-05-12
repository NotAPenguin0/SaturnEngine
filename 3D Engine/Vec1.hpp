#ifndef MVG_VEC_1_HPP_
#define MVG_VEC_1_HPP_

#include "VecBase.hpp"

#include <cassert>
#include <type_traits>

namespace Saturn {
namespace Math {
template<typename T>
class Vec<T, 1> {
public:
    static_assert(std::is_floating_point<T>::value,
                  "Saturn::Math: template parameter for Vec1 must be floating "
                  "point type");

    constexpr Vec() : x(0.0) {}
    constexpr Vec(T f) : x(f) {}
    constexpr Vec(Vec const& rhs) : x(rhs.x) {}

    constexpr Vec& operator=(Vec const& rhs) {
        x = rhs.x;
        return *this;
    }

    // x component of the vector
    T x;

    // Indexing is done using operator() to remain consistent with Matrix class
    constexpr T& operator()(std::size_t index) {
        assert(index == 0);
        return x;
    }

    constexpr T const& operator()(std::size_t index) const {
        assert(index == 0);
        return x;
    }

    constexpr T* ptr() { return &x; }
    constexpr T const* ptr() const { return &x; }
};

template<typename T>
using Vec1 = Vec<T, 1>;

// Basic operator overloads

template<typename T>
constexpr Vec1<T> operator+(Vec1<T> const& lhs, Vec1<T> const& rhs) {
    return Vec1<T>{lhs.x + rhs.x};
}

template<typename T>
constexpr Vec1<T> operator-(Vec1<T> const& lhs, Vec1<T> const& rhs) {
    return Vec1<T>{lhs.x - rhs.x};
}

template<typename T>
constexpr Vec1<T> operator*(Vec1<T> const& lhs, T scalar) {
    return Vec1<T>{lhs.x * scalar};
}

template<typename T>
constexpr Vec1<T> operator/(Vec1<T> const& lhs, T scalar) {
    return Vec1<T>{lhs.x / scalar};
}

// Comparison operators

template<typename T>
constexpr bool operator==(Vec1<T> const& lhs, Vec1<T> const& rhs) {
    return lhs.x == rhs.x;
}

} // namespace Math
} // namespace Saturn
#endif
