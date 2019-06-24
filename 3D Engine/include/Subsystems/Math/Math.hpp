#ifndef MVG_MATH_HPP_
#define MVG_MATH_HPP_

// Header that includes all other math headers

#include "ConstexprMath.hpp"
#include "Curve.hpp"
#include "NumericRange.hpp"
#include "RandomEngine.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/quaternion.hpp>

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
template<typename T>
T map_range(T input, T input_start, T input_end, T output_start, T output_end) {
    const T slope = (output_end - output_start) / (input_end - input_start);
    T output = output_start + slope * (input - input_start);
    return output;
}

inline glm::vec3 rotate_vector_by_quaternion(const glm::vec3& v,
                                             const glm::quat& q) {
    // Extract the vector part of the quaternion
    glm::vec3 u(q.x, q.y, q.z);

    // Extract the scalar part of the quaternion
    float s = q.w;

    // Do the math
    return 2.0f * glm::dot(u, v) * u + (s * s - glm::dot(u, u)) * v +
           2.0f * s * glm::cross(u, v);
}

// spherical coords in the format of {r, theta, phi}
inline glm::vec3 spherical_to_cartesian(glm::vec3 const& spherical) {
    float const& r = spherical.x;
    float const& th = spherical.y;
    float const& ph = spherical.z;
    return {r * std::sin(th) * std::cos(ph), r * std::cos(th),
            r * std::sin(th) * std::sin(ph)};
}

inline glm::vec3 spherical_to_cartesian(float r, float theta, float phi) {
    return spherical_to_cartesian({r, theta, phi});
}

inline glm::vec3 euler_to_spherical(glm::vec3 const& euler) {
    return glm::vec3(1.0f, euler.x, euler.y);
}

} // namespace Saturn::Math

#include <nlohmann/json.hpp>

namespace glm {

void to_json(::nlohmann::json& j, vec2 const& v);
void to_json(::nlohmann::json& j, vec3 const& v);
void to_json(::nlohmann::json& j, vec4 const& v);

void from_json(::nlohmann::json const& j, vec2& v);
void from_json(::nlohmann::json const& j, vec3& v);
void from_json(::nlohmann::json const& j, vec4& v);

} // namespace glm


#endif
