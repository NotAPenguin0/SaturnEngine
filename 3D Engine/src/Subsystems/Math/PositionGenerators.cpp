#include "Subsystems/Math/PositionGenerators.hpp"
#include "Subsystems/Math/Directiongenerators.hpp"
#include "Subsystems/Math/RandomEngine.hpp"
#include "Subsystems/Math/Transform.hpp"
#include "Subsystems/Math/math_traits.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Saturn::Math {

glm::vec3 random_position(glm::vec3 const& base, float max_offset) {
    float x = Math::RandomEngine::get(0.0f, max_offset);
    float y = Math::RandomEngine::get(0.0f, max_offset);
    float z = Math::RandomEngine::get(0.0f, max_offset);
    return base + glm::vec3{x, y, z};
}

glm::vec3 position_on_sphere(float radius) {
    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;
    float x = 2 * radius * std::cos(2 * pi * r1) * std::sqrt(r2 * (1 - r2));
    float y = 2 * radius * std::sin(2 * pi * r1) * std::sqrt(r2 * (1 - r2));
    float z = radius * (1 - 2 * r2);

    return {x, y, z};
}

glm::vec3 position_on_hemisphere(float radius, glm::vec3 const& rotation) {
    // Trick the system by generating a direction on a unit hemisphere and then
    // projecting it onto our hemisphere
    glm::vec3 dir = direction_in_hemisphere(2.0f, rotation);

    return dir * radius;
}

glm::vec3
position_on_circle(float radius, float arc, glm::vec3 const& rotation) {
    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    float a = r1 * glm::radians(arc);
    float r = radius * std::sqrt(r2);

    float x = r * std::cos(a);
    float y = 0.0f;
    float z = r * std::sin(a);

    return Math::rotate_vector_by_quaternion(glm::vec3(x, y, z),
                                             glm::quat(rotation));
}

glm::vec3 position_in_box(glm::vec3 const& scale, glm::vec3 const& rotation) {
    glm::vec3 min = -(scale / 2.0f);
    glm::vec3 max = -min;
    float x = Math::RandomEngine::get(min.x, max.x);
    float y = Math::RandomEngine::get(min.y, max.y);
    float z = Math::RandomEngine::get(min.z, max.z);
    return Math::rotate_vector_by_quaternion(glm::vec3(x, y, z),
                                             glm::quat(rotation));
}

} // namespace Saturn::Math
