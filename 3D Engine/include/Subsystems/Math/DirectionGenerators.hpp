#ifndef MVG_DIRECTION_GENERATOR_HPP_
#define MVG_DIRECTION_GENERATOR_HPP_

#include <glm/glm.hpp>

namespace Saturn::Math {

glm::vec3 random_direction(glm::vec3 const& base, float randomness);
// base_rotation in euler angles
glm::vec3 direction_in_sphere(float randomness, glm::vec3 const& base_rotation);
glm::vec3 direction_in_hemisphere(float randomness,
                                  glm::vec3 base = {0.0f, 1.0f, 0.0f});
glm::vec3 direction_in_cone(float arc, float angle, glm::vec3 const& rotation);

}

#endif