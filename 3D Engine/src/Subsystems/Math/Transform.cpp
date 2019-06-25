#include "Subsystems/Math/Transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace glm {

inline mat4 rotate(mat4 const& mat, vec3 euler) {
    mat4 result;
    result = rotate(mat, euler.z, vec3{0, 0, 1});
    result = rotate(result, euler.y, vec3{0, 1, 0});
    result = rotate(result, euler.x, vec3{1, 0, 0});
    return result;
}

} // namespace glm

namespace Saturn::Math {

glm::vec3 rotate_vector_by_quaternion(const glm::vec3& v, const glm::quat& q) {
    // Extract the vector part of the quaternion
    glm::vec3 u(q.x, q.y, q.z);

    // Extract the scalar part of the quaternion
    float s = q.w;

    // Do the math
    return 2.0f * glm::dot(u, v) * u + (s * s - glm::dot(u, u)) * v +
           2.0f * s * glm::cross(u, v);
}

} // namespace Saturn::Math
