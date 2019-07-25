#include "Subsystems/Physics/CollisionDetectors.hpp"

#include "Utility/Utility.hpp"

namespace Saturn {

static bool range_overlaps(float min_a, float max_a, float min_b, float max_b) {
    return min_a <= max_b && max_a >= min_b;
}

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection.
// Very simple aabb intersection

bool point_inside_aabb(glm::vec3 const& point,
                       Components::BoxCollider const& aabb,
                       Components::Transform const& transform) {
    // A point is inside an AABB if the distance from that point to the AABB's
    // center on each axis is smaller than the half_width of that axis
    glm::vec3 const center =
        make_absolute_transform(transform).position + aabb.center;
    glm::vec3 const& widths = aabb.half_widths;

    glm::vec3 distance = point - center;
    // clang-format off
    return glm::abs(distance.x) < widths.x 
		&& glm::abs(distance.y) < widths.y 
		&& glm::abs(distance.z) < widths.z;
    // clang-format on
}

bool aabb_intersects_aabb(Components::BoxCollider const& first,
                          Components::Transform const& first_trans,
                          Components::BoxCollider const& second,
                          Components::Transform const& second_trans) {
    // #TODO: Figure out a smarter way, like with point_inside_aabb()

    glm::vec3 const center1 =
        make_absolute_transform(first_trans).position + first.center;
    glm::vec3 const center2 =
        make_absolute_transform(second_trans).position + second.center;

    glm::vec3 min1 = center1 - first.half_widths;
    glm::vec3 max1 = center1 + first.half_widths;

    glm::vec3 min2 = center2 - second.half_widths;
    glm::vec3 max2 = center2 + second.half_widths;

    bool x = max1.x >= min2.x && min1.x <= max2.x;
    bool y = max1.y >= min2.y && min1.y <= max2.y;
    bool z = max1.z >= min2.z && min1.z <= max2.z;

    return x && y && z;
}

} // namespace Saturn
