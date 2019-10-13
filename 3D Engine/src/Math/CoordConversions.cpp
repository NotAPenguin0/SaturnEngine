#include "Math/CoordConversions.hpp"

namespace Saturn::Math {
// spherical coords in the format of {r, theta, phi}
glm::vec3 spherical_to_cartesian(glm::vec3 const& spherical) {
    float const& r = spherical.x;
    float const& th = spherical.y;
    float const& ph = spherical.z;
    return {r * std::sin(th) * std::cos(ph), r * std::cos(th),
            r * std::sin(th) * std::sin(ph)};
}

glm::vec3 spherical_to_cartesian(float r, float theta, float phi) {
    return spherical_to_cartesian({r, theta, phi});
}

glm::vec3 euler_to_spherical(glm::vec3 const& euler) {
    return glm::vec3(1.0f, euler.x, euler.y);
}
}