#ifndef MVG_MATH_HPP_
#define MVG_MATH_HPP_

// Header that includes all other math headers

#include "ConstexprMath.hpp"
#include "RandomEngine.hpp"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace glm {

inline mat4 rotate(mat4 const& mat, vec3 euler) {
	mat4 result;
    result = rotate(mat, euler.z, vec3{0, 0, 1});
    result = rotate(result, euler.y, vec3{0, 1, 0});
    result = rotate(result, euler.x, vec3{1, 0, 0});
    return result;
}

} // namespace glm
#endif
