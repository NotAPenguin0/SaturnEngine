#include "Subsystems/Math/Curve.hpp"

#include <cassert>

namespace Saturn::Math {

float Curve::get(float x, float x_max) const {
    assert(x <= x_max);
    switch (shape) {
        case CurveShape::Constant: return scale;
        case CurveShape::LinearUp: return x * scale;
        case CurveShape::LinearDown: return (x_max - x) * scale;
    }
}

} // namespace Saturn::Math
