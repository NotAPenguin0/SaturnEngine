#include "Utility/Utility.hpp"

#include "Scene/Scene.hpp"

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
        // Parenting should not affect scale
        //        new_trans.scale *= parent_trans.scale;

        return new_trans;
    }
}

std::vector<float> make_float_vec(std::vector<glm::vec3> const& v) {
    std::vector<float> result;
    result.reserve(v.size() * 3);
    for (auto& elem : v) {
        result.push_back(elem.x);
        result.push_back(elem.y);
        result.push_back(elem.z);
    }
    return result;
}

std::vector<float> make_float_vec(std::vector<glm::vec4> const& v) {
    std::vector<float> result;
    result.reserve(v.size() * 4);
    for (auto& elem : v) {
        result.push_back(elem.x);
        result.push_back(elem.y);
        result.push_back(elem.z);
        result.push_back(elem.w);
    }
    return result;
}

} // namespace Saturn
