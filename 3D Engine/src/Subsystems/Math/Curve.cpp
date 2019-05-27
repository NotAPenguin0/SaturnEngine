#include "Subsystems/Math/Curve.hpp"

#include <cassert>

namespace Saturn::Math {

float Curve::get(float x) const {
    switch (shape) {
        case CurveShape::Constant: return min;
        case CurveShape::LinearUp: return x * (max - min) + min;
        case CurveShape::LinearDown: return max - x * (max - min);
    }
}

numeric_range<float> Curve::output_range() const {
    switch (shape) {
        case CurveShape::Constant: return {min, min};
        case CurveShape::LinearUp: return {min, max};
        case CurveShape::LinearDown: return {min, max};
    }
}

} // namespace Saturn::Math
