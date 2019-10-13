#ifndef SATURN_CONSTEXPR_MATH_HPP_
#define SATURN_CONSTEXPR_MATH_HPP_

#include <cassert>
#include <cstddef>

namespace Saturn {
namespace Math {
template<typename T>
constexpr T abs(T val) {
    if (val < 0) return -val;
    return val;
}

template<typename T>
constexpr bool approx(T a, T b, T epsilon = (T)0.0001) {
    return abs(a - b) < epsilon;
}

template<typename T>
constexpr T sqrt(T value) {
    constexpr std::size_t NumIterations = 25;

    if (approx(value, 0.0f)) return 0;

    // Newton's method of determining root of a function
    T guess = value;
    for (std::size_t i = 0; i < NumIterations; ++i) {
        guess -= (guess * guess - value) / (2 * guess);
    }

    return guess;
}

} // namespace Math
} // namespace Saturn

#endif
