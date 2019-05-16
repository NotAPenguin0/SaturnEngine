#include "Subsystems/Scene/Scene.hpp"

#include "Subsystems/Scene/SceneObject.hpp"

namespace Saturn {

Scene::Scene() : ecs(this) {}

Scene::~Scene() {}

void Scene::update_systems() { ecs.update_systems(); }
void Scene::on_start() { ecs.on_start(); }


SceneObject& Scene::create_object(SceneObject* parent /* = nullptr*/) {
    objects.push_back(std::make_unique<SceneObject>(this, parent));
    return *objects.back();
}

ECS<COMPONENT_LIST>& Scene::get_ecs() { return ecs; }

} // namespace Saturn
