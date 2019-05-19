#ifndef MVG_CURVE_HPP_
#define MVG_CURVE_HPP_

namespace Saturn::Math {

enum class CurveShape { Constant, LinearUp, LinearDown };

struct Curve {
    CurveShape shape = CurveShape::Constant;
    float scale = 1.0f;

	float get(float x, float x_max) const;
};

} // namespace Saturn::Math

#endif
