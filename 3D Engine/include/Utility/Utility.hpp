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

Components::Transform
make_absolute_transform(Components::Transform const& old_transform);

std::vector<float> make_float_vec(std::vector<glm::vec4> const& v);
std::vector<float> make_float_vec(std::vector<glm::vec3> const& v);

namespace detail {

template<std::size_t I, typename T, typename... Ts>
struct index_of_impl;

template<std::size_t I, typename T, typename Head, typename... Tail>
struct index_of_impl<I, T, Head, Tail...> {
    static constexpr std::size_t value =
        std::is_same<T, Head>::value ? I
                                     : index_of_impl<I + 1, T, Tail...>::value;
};

// Fallback if not found or last element to end recursion
template<std::size_t I, typename T, typename Head>
struct index_of_impl<I, T, Head> {
    static constexpr std::size_t value =
        std::is_same<T, Head>::value ? I : static_cast<std::size_t>(-1);
};
} // namespace detail

template<typename T, typename... Ts>
struct index_of {
    static constexpr std::size_t value = detail::index_of_impl<0, T, Ts...>::value;
};

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

} // namespace Saturn

#endif
