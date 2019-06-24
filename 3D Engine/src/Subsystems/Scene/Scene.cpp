#include "Subsystems/Scene/Scene.hpp"

#include "Subsystems/Scene/SceneObject.hpp"

#include <fstream>

namespace Saturn {

Scene::Scene() : ecs(this) {}

Scene::~Scene() {}

void Scene::update_systems() { ecs.update_systems(); }
void Scene::on_start() { ecs.on_start(); }

SceneObject& Scene::create_object(SceneObject* parent /* = nullptr*/) {
    objects.push_back(std::make_unique<SceneObject>(this, parent));
    return *objects.back();
}

SceneObject&
Scene::create_object_from_file(std::string_view file_path,
                               SceneObject* parent /* = nullptr */) {
    auto& object = create_object();
    std::ifstream f(file_path.data());
    nlohmann::json j;
    f >> j;
    j.get_to(object);
    object.parent_obj = parent;
	object.scene = this;
    return object;
}

ECS<COMPONENT_LIST>& Scene::get_ecs() { return ecs; }

} // namespace Saturn
