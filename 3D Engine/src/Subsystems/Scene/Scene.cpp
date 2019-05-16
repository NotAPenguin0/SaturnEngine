#include "Subsystems\Scene\Scene.hpp"

#include "Subsystems\Scene\SceneObject.hpp"

namespace Saturn {

Scene::Scene() : ecs(this) {}

Scene::~Scene() {}

void Scene::update_systems() { ecs.update_systems(); }
void Scene::on_start() { ecs.on_start(); }

SceneGraph Scene::build_scene_graph() {
    // Temporary
    SceneGraph graph;
    graph.scene = this;
    for (auto[transform, mesh] : ecs.select<Components::Transform, Components::StaticMesh>()) {
        graph.transforms.push_back(&transform);
        graph.meshes.push_back(mesh.mesh);
        graph.shader = transform.entity->debug.shader;
    }

    return graph;
}

SceneObject& Scene::create_object() {
    objects.push_back(std::make_unique<SceneObject>(this));
    return *objects.back();
}

ECS<COMPONENT_LIST>& Scene::get_ecs() { return ecs; }

} // namespace Saturn
