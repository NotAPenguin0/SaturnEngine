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

numeric_range<float> Curve::output_range(float xmax) const {
    switch (shape) {
        case CurveShape::Constant: return {0.0f, scale};
        case CurveShape::LinearUp: return {0.0f, xmax * scale};
        case CurveShape::LinearDown: return {0.0f, xmax * scale};
    }
}

} // namespace Saturn::Math
