#ifndef SATURN_COLLISION_DETECTORS_HPP_
#define SATURN_COLLISION_DETECTORS_HPP_

#include <glm/glm.hpp>

#include "ECS/Components/BoxCollider.hpp"
#include "ECS/Components/Transform.hpp"

namespace Saturn {

bool point_inside_aabb(glm::vec3 const& point,
                       Components::BoxCollider const& aabb,
                       Components::Transform const& transform);
bool aabb_intersects_aabb(Components::BoxCollider const& first, Components::Transform const& first_trans,
                          Components::BoxCollider const& second, Components::Transform const& second_trans);

} // namespace Saturn

#endif
