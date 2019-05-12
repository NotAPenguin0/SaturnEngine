#ifndef MVG_PI_HPP_
#define MVG_PI_HPP_

namespace Saturn {
namespace Math {

template<typename T>
struct math_traits;

template<>
struct math_traits<float> {
    static constexpr float pi = 3.1415927f;
};

template<>
struct math_traits<double> {
    static constexpr double pi = 3.14159265358979323846;
};

} // namespace Math

} // namespace Saturn

#endif
