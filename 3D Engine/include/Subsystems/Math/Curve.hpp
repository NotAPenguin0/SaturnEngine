#ifndef MVG_CURVE_HPP_
#define MVG_CURVE_HPP_

#include "NumericRange.hpp"

namespace Saturn::Math {

enum class CurveShape { Constant, LinearUp, LinearDown };

struct Curve {
    CurveShape shape = CurveShape::Constant;
    float scale = 1.0f;

    float get(float x, float x_max) const;

	numeric_range<float> output_range(float xmax) const;
};

} // namespace Saturn::Math

#endif
