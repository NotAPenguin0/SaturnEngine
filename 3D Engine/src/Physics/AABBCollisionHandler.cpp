#include "Physics/AABBCollisionHandler.hpp"

#include "Input/Input.hpp"
#include "Physics/CollisionDetectors.hpp"
#include "Scene/SceneObject.hpp"

namespace Saturn {

using namespace Components;

AABBCollisionHandler::Collision AABBCollisionHandler::detect_collision(
    Rigidbody& rb, BoxCollider& first, BoxCollider& second) {

    auto& trans1 = first.entity->get_component<Transform>();
    auto& trans2 = second.entity->get_component<Transform>();

    bool collided = aabb_intersects_aabb(first, trans1, second, trans2);
    return Collision{collided, &rb, &first, &second};
}


} // namespace Saturn
