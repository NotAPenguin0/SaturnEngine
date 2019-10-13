#ifndef SATURN_COORD_CONVERSIONS_HPP_
#define SATURN_COORD_CONVERSIONS_HPP_

#include <glm/glm.hpp>

namespace Saturn::Math {

// spherical coords in the format of {r, theta, phi}
glm::vec3 spherical_to_cartesian(glm::vec3 const& spherical);
glm::vec3 spherical_to_cartesian(float r, float theta, float phi);
glm::vec3 euler_to_spherical(glm::vec3 const& euler);


}

#endif