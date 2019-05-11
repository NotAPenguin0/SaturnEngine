#include "Scene.hpp"

#include "SceneObject.hpp"
#include "Trig.hpp"

namespace Saturn {

Scene::Scene() : ecs(this) {}

Scene::~Scene() {}

void Scene::update_systems() { ecs.update_systems(); }

SceneGraph Scene::build_scene_graph() {
    // Temporary
    SceneGraph graph;
    graph.scene = this;
    auto& transforms = ecs.get_components<Components::Transform>();
    for (auto& t : transforms) {
        auto& entity = t.entity;
        graph.transforms.push_back(&t);
        graph.vtx_arrays.push_back(&entity->debug.cube);
        graph.shader = &entity->debug.shader;
    }

    return graph;
}

SceneObject& Scene::create_object() {
    objects.push_back(std::make_unique<SceneObject>(this));
    return *objects.back();
}

ECS<COMPONENT_LIST>& Scene::get_ecs() { return ecs; }

} // namespace Saturn
