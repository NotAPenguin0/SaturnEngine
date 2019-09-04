#ifndef SATURN_UTILITY_HPP_
#define SATURN_UTILITY_HPP_

#include "Subsystems/ECS/Components/Transform.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
#include <cstddef>

#include <sstream>
#include <vector>

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

template<typename string_type>
std::vector<string_type> split(string_type const& s,
                               typename string_type::value_type delim) {
    std::vector<string_type> elems;
    std::basic_stringstream<typename string_type::value_type> ss(s);
    string_type item;
    while (std::getline(ss, item, delim)) { elems.push_back(item); }
    return elems;
}

template<typename string_type>
string_type join(std::vector<string_type> const& v,
                 typename string_type::value_type sep = ' ') {
    string_type result;
    for (auto elem : v) {
        result += elem;
        result += sep;
    }
    result.pop_back();
    return result;
}

template<typename string_type>
bool string_match_n(string_type const& a, string_type const& b, std::size_t n) {
    if (n < a.size() || n < b.size()) return false;
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

namespace detail {
struct comp_eq {
    template<typename T>
    bool operator()(T const& a, T const& b) {
        return a == b;
    }
};
} // namespace detail

template<typename string_type, typename Comp>
bool string_match_n(string_type const& a,
                    string_type const& b,
                    std::size_t n,
                    Comp&& compare = detail::comp_eq{}) {
    if (n > a.size() || n > b.size()) return false;
    for (std::size_t i = 0; i < n; ++i) {
        if (!compare(a[i], b[i])) return false;
    }
    return true;
}

template<typename InputIt, typename F>
bool all_satisfy_condition(InputIt begin, InputIt end, F&& condition) {
    for (; begin != end; ++begin) {
        if (!condition(*begin)) return false;
    }
    return true;
};

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
    static constexpr std::size_t value =
        detail::index_of_impl<0, T, Ts...>::value;
};

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

} // namespace Saturn

#endif
