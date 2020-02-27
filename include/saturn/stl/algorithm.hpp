#ifndef SATURN_STL_ALGORITHM_HPP_
#define SATURN_STL_ALGORITHM_HPP_

#include <saturn/stl/types.hpp>

namespace stl {

template<typename InputIt, typename OutputIt>
void copy_n(InputIt src, OutputIt dst, size_t n) {
    while(n-- > 0) {
        *dst++ = *src++;
    }
}

template<typename InputIt, typename OutputIt>
void copy(InputIt begin, InputIt end, OutputIt dst) {
    while(begin != end) {
        *dst++ = *begin++;
    }
}

template<typename T, typename U>
T min(T const& a, U const& b) {
    return a < b ? a : b;
}

template<typename T, typename U>
T max(T const& a, U const& b) {
    return b < a ? a : b;
}

}

#endif