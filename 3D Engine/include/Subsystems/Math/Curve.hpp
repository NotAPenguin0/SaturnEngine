#ifndef MVG_CURVE_HPP_
#define MVG_CURVE_HPP_

#include "NumericRange.hpp"

namespace Saturn::Math {

enum class CurveShape { Constant, LinearUp, LinearDown };

struct Curve {
    CurveShape shape = CurveShape::Constant;
    float min;
    float max;

    float get(float x) const;

    numeric_range<float> output_range() const;
};

} // namespace Saturn::Math

#endif
