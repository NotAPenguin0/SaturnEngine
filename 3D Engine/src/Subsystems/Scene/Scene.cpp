#include "Subsystems/Scene/Scene.hpp"

#include "Core/Application.hpp"
#include "Subsystems/Scene/SceneObject.hpp"

#include <filesystem>
#include <fstream>

namespace Saturn {

Scene::Scene(Application* app) : ecs(this), app(app) {}

Scene::~Scene() {}

void Scene::update_systems() { ecs.update_systems(); }
void Scene::on_start() { ecs.on_start(); }

Application* Scene::get_app() { return app; }

SceneObject& Scene::create_object(SceneObject* parent /* = nullptr*/) {
    objects.push_back(std::make_unique<SceneObject>(this, parent));
    objects.back()->set_id(IDGenerator<SceneObject>::next());
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

void Scene::serialize_to_file(std::string_view folder) {
    namespace fs = std::filesystem;
    using namespace Components;
    fs::create_directories(folder.data() + std::string("/entities"));
    std::ofstream file(folder.data() + std::string("/scene.dat"));
    for (std::size_t i = 0; i < objects.size(); ++i) {
        std::string fname = folder.data() + std::string("/entities/");
        if (objects[i]->has_component<Name>()) {
            fname += (objects[i]->get_component<Name>().name + ".json");
        } else {
            fname += std::to_string(i) + ".json";
        }
        objects[i]->serialize_to_file(fname);
        file << fname << "\n";
    }
}

void Scene::deserialize_from_file(std::string_view path) {
    std::ifstream file(path.data());
    std::string fname;
    while (std::getline(file, fname)) { create_object_from_file(fname); }
    resolve_parent_pointers();
    set_id_generator_value();
}

ECS<COMPONENT_LIST>& Scene::get_ecs() { return ecs; }

void Scene::resolve_parent_pointers() {
    for (auto& obj : objects) {
        if (obj->get_parent_id() != IDGenerator<SceneObject>::none) {
            auto parent_id = obj->get_parent_id();
            for (auto& parent_candidate : objects) {
                if (parent_candidate->get_id() == parent_id) {
                    obj->set_parent(parent_candidate.get());
                }
            }
        }
    }
}

void Scene::set_id_generator_value() {
    std::size_t max_id = 0;
    for (auto const& obj : objects) {
        if (std::size_t id = obj->get_id(); id > max_id) { max_id = id; }
    }
    IDGenerator<SceneObject>::set(max_id);
}

} // namespace Saturn
