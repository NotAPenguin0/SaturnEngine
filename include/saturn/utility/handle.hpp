#ifndef SATURN_UTILITY_HANDLE_HPP_
#define SATURN_UTILITY_HANDLE_HPP_

#include <stl/types.hpp>

namespace saturn {

template<typename T>
struct Handle {
    stl::int64_t id = -1;
};

} // namespace saturn


#endif