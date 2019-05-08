#include "Scene.hpp"

#include "SceneObject.hpp"
#include "Trig.hpp"

namespace Saturn {

Scene::Scene() {
    auto& obj = objects.emplace_back(std::make_unique<SceneObject>());
    auto& transforms = ecs.get_components<Components::Transform>();
    transforms.push_back(
        Components::Transform{{obj.get()},
                              Math::Vec3<float>(0.0f, 0.0f, -10.0f), // Position
                              {
                                  Math::Vec3<float>(0.0f, 1.0f, 0.0f), // Rotation: Axis
                                  Math::radians(45.0f), // Rotation: Angle
                              },
                              Math::Vec3<float>(1.3f, 1.3f, 1.3f)}); // Scale
}

SceneGraph Scene::build_scene_graph() {
    // Temporary
    SceneGraph graph;
    auto& transforms = ecs.get_components<Components::Transform>();
    for (auto& t : transforms) {
        auto& entity = t.object;
        graph.transforms.push_back(&t);
        graph.vtx_arrays.push_back(&entity->debug.cube);
        graph.shader = &entity->debug.shader;
    }

    return graph;
}

} // namespace Saturn
