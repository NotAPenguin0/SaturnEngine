#ifndef SATURN_TRANSFORM_HPP_
#define SATURN_TRANSFORM_HPP_

#include <glm/glm.hpp>

namespace glm {

mat4 rotate(mat4 const& mat, vec3 euler);

} // namespace glm

namespace Saturn::Math {

glm::vec3 rotate_vector_by_quaternion(const glm::vec3& v, const glm::quat& q);

} // namespace Saturn::Math

#endif
