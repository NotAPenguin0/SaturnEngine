#include "Subsystems\Scene\Scene.hpp"

#include "Subsystems\Scene\SceneObject.hpp"

namespace Saturn {

// Temp, might want to move this to the renderer later
static Components::Transform make_absolute_transform(SceneObject* object) {
    if (!object->has_parent()) {
        return object->get_component<Components::Transform>();
    } else {
        auto parent = object->parent();
        auto trans = object->get_component<Components::Transform>();
        auto parent_trans = make_absolute_transform(parent);

		Components::Transform new_trans = trans;
        new_trans.position += parent_trans.position;
        new_trans.rotation += parent_trans.rotation;
        new_trans.scale *= parent_trans.scale;

		return new_trans;
    }
}

Scene::Scene() : ecs(this) {}

Scene::~Scene() {}

void Scene::update_systems() { ecs.update_systems(); }
void Scene::on_start() { ecs.on_start(); }

//#BigTODO: Get rid of SceneGraph, only pass ECS, Renderer works with that

SceneGraph Scene::build_scene_graph() {
    // Temporary
    SceneGraph graph;
    graph.scene = this;
    for (auto [transform, mesh] :
         ecs.select<Components::Transform, Components::StaticMesh>()) {
        graph.transforms.push_back(make_absolute_transform(transform.entity)); // Temp
        graph.meshes.push_back(mesh.mesh);
        graph.shader = transform.entity->debug.shader;
    }

    return graph;
}

SceneObject& Scene::create_object(SceneObject* parent /* = nullptr*/) {
    objects.push_back(std::make_unique<SceneObject>(this, parent));
    return *objects.back();
}

ECS<COMPONENT_LIST>& Scene::get_ecs() { return ecs; }

} // namespace Saturn
