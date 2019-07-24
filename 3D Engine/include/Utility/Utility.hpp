#ifndef SATURN_UTILITY_HPP_
#define SATURN_UTILITY_HPP_

#include "Subsystems/ECS/Components/Transform.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
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

Components::Transform make_absolute_transform(Components::Transform const& old_transform);

std::vector<float> make_float_vec(std::vector<glm::vec4> const& v);
std::vector<float> make_float_vec(std::vector<glm::vec3> const& v);

} // namespace Saturn

#endif
