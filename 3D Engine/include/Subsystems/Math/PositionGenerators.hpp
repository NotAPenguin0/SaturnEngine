#ifndef MVG_POSITION_GENERATORS_HPP_
#define MVG_POSITION_GENERATORS_HPP_

#include <glm/glm.hpp>

namespace Saturn::Math {

glm::vec3 random_position(glm::vec3 const& base, float max_offset);
glm::vec3 position_on_sphere(float radius);
glm::vec3 position_on_hemisphere(float radius, glm::vec3 const& rotation);

glm::vec3
position_on_circle(float radius, float arc, glm::vec3 const& rotation);

// scale specifies how to scale the box centered (0, 0, 0) with size(1, 1,
// 1)
glm::vec3 position_in_box(glm::vec3 const& scale, glm::vec3 const& rotation);

} // namespace Saturn::Math

#endif
