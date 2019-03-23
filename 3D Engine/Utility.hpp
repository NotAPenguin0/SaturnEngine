#ifndef MVG_UTILITY_HPP_
#define MVG_UTILITY_HPP_

#include <cstddef>

namespace Saturn {

template<typename T>
struct Dimensions {
    Dimensions() = delete;
    Dimensions(T x, T y) : x(x), y(x) {}
    Dimensions(Dimensions const&) = default;
    Dimensions& operator=(Dimensions const&) = default;
    ~Dimensions() = default;
    T x, y;
};

using WindowDim = Dimensions<std::size_t>;
using ImgDim = Dimensions<std::size_t>;

} // namespace Saturn

#endif
