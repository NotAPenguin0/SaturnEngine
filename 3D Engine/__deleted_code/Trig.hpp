#ifndef MVG_TRIG_HPP_
#define MVG_TRIG_HPP_

#include "math_traits.hpp"

namespace Saturn {

namespace Math {

template<typename T>
T radians(T degrees) {
    return degrees * (math_traits<T>::pi / (T)180.0);
}

} // namespace Math

} // namespace Saturn

#endif
