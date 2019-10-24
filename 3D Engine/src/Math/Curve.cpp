#include "Math/Curve.hpp"

#include <cassert>

#include <cmath>

namespace Saturn::Math {

float Curve::get(float x) const {
    switch (shape) {
        case CurveShape::Constant: return min;
        case CurveShape::LinearUp: return x * (max - min) + min;
        case CurveShape::LinearDown: return max - x * (max - min);
        case CurveShape::Sin: return std::sin(x * 3.141592f) * (max - min) + min;
    }
}

numeric_range<float> Curve::output_range() const {
    switch (shape) {
        case CurveShape::Constant: return {min, min};
        case CurveShape::LinearUp: return {min, max};
        case CurveShape::LinearDown: return {min, max};
        case CurveShape::Sin: return {min, max};
    }
}

void from_json(nlohmann::json const& j, Curve& curve) {
    curve.min = j["Min"].get<float>();
    curve.max = j["Max"].get<float>();
    auto shape_str = j["CurveShape"].get<std::string>();
    if (shape_str == "Constant") curve.shape = CurveShape::Constant;
    if (shape_str == "LinearUp") curve.shape = CurveShape::LinearUp;
    if (shape_str == "LinearDown") curve.shape = CurveShape::LinearDown;
    if (shape_str == "Sin") curve.shape = CurveShape::Sin;
}

void to_json(nlohmann::json& j, Curve const& curve) {
    j["Min"] = curve.min;
    j["Max"] = curve.max;
    std::string shape;
    switch (curve.shape) {
        case CurveShape::Constant: shape = "Constant"; break;
        case CurveShape::LinearUp: shape = "LinearUp"; break;
        case CurveShape::LinearDown: shape = "LinearDown"; break;
        case CurveShape::Sin: shape = "Sin"; break;
    }
    j["CurveShape"] = shape;
}

} // namespace Saturn::Math
