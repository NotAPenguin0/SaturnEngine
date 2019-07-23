#ifndef MVG_MATH_HPP_
#define MVG_MATH_HPP_

// Header that includes all other math headers

#include "ConstexprMath.hpp"
#include "CoordConversions.hpp"
#include "Curve.hpp"
#include "DirectionGenerators.hpp"
#include "Misc.hpp"
#include "NumericRange.hpp"
#include "PositionGenerators.hpp"
#include "RandomEngine.hpp"
#include "Transform.hpp"
#include "math_traits.hpp"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace glm {

void to_json(::nlohmann::json& j, vec2 const& v);
void to_json(::nlohmann::json& j, vec3 const& v);
void to_json(::nlohmann::json& j, vec4 const& v);
void to_json(::nlohmann::json& j, bvec3 const& v);

void from_json(::nlohmann::json const& j, vec2& v);
void from_json(::nlohmann::json const& j, vec3& v);
void from_json(::nlohmann::json const& j, vec4& v);
void from_json(::nlohmann::json const& j, bvec3& v);

bvec3 negate(bvec3 const& vec);

} // namespace glm

#endif
