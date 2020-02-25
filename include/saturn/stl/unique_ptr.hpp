#ifndef SATURN_STL_UNIQUE_PTR_HPP_
#define SATURN_STL_UNIQUE_PTR_HPP_

#include <memory>

namespace stl {

template<typename T>
using unique_ptr = std::unique_ptr<T>;

}

#endif