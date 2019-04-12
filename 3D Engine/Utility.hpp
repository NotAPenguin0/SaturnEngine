#ifndef MVG_UTILITY_HPP_
#define MVG_UTILITY_HPP_

#include "LogSystem.hpp"
#include <cstddef>

namespace Saturn {

#define log_function_info(sev)                                                 \
    ::Saturn::LogSystem::write(sev, __PRETTY_FUNCTION__);

// Do not catch this exception anywhere except at the end of main
struct SafeTerminateException {};

[[noreturn]] inline void safe_terminate() { throw SafeTerminateException{}; }

template<typename T>
struct Dimensions {
    T x, y;
};

using WindowDim = Dimensions<std::size_t>;
using ImgDim = Dimensions<std::size_t>;

// Vertex Array Object type
using Vao = unsigned int;
// Vertex Buffer Object type
using Vbo = unsigned int;

// Basic color class. Needs expanding
class Color {
public:
    Color() = default;
    Color(float aR, float aG, float aB, float aA);
    Color(Color const&) = default;
    Color(Color&&) = default;

    Color& operator=(Color const&) = default;
    Color& operator=(Color&&) = default;

    float r, g, b, a;
};

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(NonCopyable const&) = delete;
    NonCopyable& operator=(NonCopyable const&) = delete;
};

} // namespace Saturn

#endif
