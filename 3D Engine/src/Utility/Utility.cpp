#include "Utility/Utility.hpp"

#include "Subsystems/Scene/Scene.hpp"

namespace Saturn {

Color::Color(float aR, float aG, float aB, float aA) :
    r(aR), g(aG), b(aB), a(aA) {}

Components::Transform
make_absolute_transform(Components::Transform const& old_transform) {
    auto* object = old_transform.entity;
    if (!object->has_parent()) {
        return old_transform;
    } else {
        auto parent = object->parent();
        auto parent_trans = make_absolute_transform(
            parent->get_component<Components::Transform>());

        Components::Transform new_trans = old_transform;
        new_trans.position += parent_trans.position;
        new_trans.rotation += parent_trans.rotation;
        new_trans.scale *= parent_trans.scale;

        return new_trans;
    }
}

} // namespace Saturn
