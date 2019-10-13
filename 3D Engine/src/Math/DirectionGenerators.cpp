#include "Math/DirectionGenerators.hpp"
#include "Math/CoordConversions.hpp"
#include "Math/RandomEngine.hpp"
#include "Math/Transform.hpp"
#include "Math/math_traits.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Saturn::Math {
glm::vec3 random_direction(glm::vec3 const& base, float randomness) {
    glm::vec3 direction = Math::spherical_to_cartesian(base);

    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    float theta_max = pi * randomness;
    float x = std::cos(2 * pi * r1) *
              std::sqrt(1 - std::pow((1 - r2 * (1 - std::cos(theta_max))), 2));
    float z = std::sin(2 * pi * r1) *
              std::sqrt(1 - std::pow((1 - r2 * (1 - std::cos(theta_max))), 2));
    float y = 1 - r2 * (1 - std::cos(theta_max));

    glm::quat rotation =
        glm::rotation(glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3(x, y, z));

    return Math::rotate_vector_by_quaternion(direction, rotation);
}

glm::vec3 direction_in_sphere(float randomness,
                              glm::vec3 const& base_rotation) {
    const glm::vec3 base = Math::euler_to_spherical(base_rotation);
    return random_direction(base, randomness);
}

glm::vec3 direction_in_hemisphere(float randomness, glm::vec3 base) {
    // Divide by 4 to transform given randomness for a full sphere to the
    // hemisphere
    return random_direction(Math::euler_to_spherical(base), randomness / 4.0f);
}

glm::vec3 direction_in_cone(float arc, float angle, glm::vec3 const& rotation) {
    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    // fixed theta value because we're on a cone
    float theta = glm::radians(angle);
    // Random phi value on our defined arc
    float phi = r1 * glm::radians(arc);
    return Math::rotate_vector_by_quaternion(
        Math::spherical_to_cartesian(1.0f, theta, phi), glm::quat(rotation));
}

} // namespace Saturn::Math
