#ifndef MVG_UTILITY_HPP_
#define MVG_UTILITY_HPP_

#include "LogSystem.hpp"
#include <cstddef>

namespace Saturn {

#define log_function_info(sev)                                                 \
    ::Saturn::LogSystem::write(sev, __PRETTY_FUNCTION__);

struct SafeTerminateException {};

[[noreturn]] inline void safe_terminate() { throw SafeTerminateException {}; }

template<typename T>
struct Dimensions {
    T x, y;
};

using WindowDim = Dimensions<std::size_t>;
using ImgDim = Dimensions<std::size_t>;

} // namespace Saturn

#endif
