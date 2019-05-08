#ifndef MVG_CONSTEXPR_MATH_HPP_
#define MVG_CONSTEXPR_MATH_HPP_

#include <cassert>
#include <cstddef>

namespace Saturn::Math {

template<typename T>
constexpr T sqrt(T value) {
    constexpr std::size_t NumIterations = 25;

    assert(value != 0);
    if (value == 0) return 0;

	// Newton's method of determining root of a function
    T guess = value;
    for (std::size_t i = 0; i < NumIterations; ++i) {
        guess -= (guess * guess - value) / (2 * guess);
    }

	return guess;
}

} // namespace Saturn::Math

#endif
