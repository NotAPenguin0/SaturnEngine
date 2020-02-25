#ifndef SATURN_STL_VECTOR_HPP_
#define SATURN_STL_VECTOR_HPP_

#include <vector>

namespace stl {

template<typename T, typename Alloc = std::allocator<T>>
using vector = std::vector<T, Alloc>;

}

#endif