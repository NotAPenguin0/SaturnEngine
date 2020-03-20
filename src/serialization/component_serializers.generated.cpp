#include <saturn/serialization/component_serializers.hpp>
#include <saturn/serialization/default_serializers.hpp>

#include <phobos/renderer/material.hpp>
#include <phobos/renderer/mesh.hpp>
#include <phobos/renderer/texture.hpp>

#include <stl/tuple.hpp>

#include <saturn/scene/scene.hpp>

namespace saturn::components {

void from_json(nlohmann::json const& j, Blueprint& component) {
    component.blueprint = j["blueprint"];
    component.model = j["model"];
}

void to_json(nlohmann::json& j, Blueprint const& component) {
    j["blueprint"] = component.blueprint;
    j["model"] = component.model;
}


void from_json(nlohmann::json const& j, BlueprintInstance& component) {
    component.blueprint = j["blueprint"];
}

void to_json(nlohmann::json& j, BlueprintInstance const& component) {
    j["blueprint"] = component.blueprint;
}


void from_json(nlohmann::json const& j, Camera& component) {
    component.front = j["front"];
    component.up = j["up"];
    component.fov = j["fov"];
}

void to_json(nlohmann::json& j, Camera const& component) {
    j["front"] = component.front;
    j["up"] = component.up;
    j["fov"] = component.fov;
}


void from_json(nlohmann::json const& j, MeshRenderer& component) {
    component.material = j["material"];
}

void to_json(nlohmann::json& j, MeshRenderer const& component) {
    j["material"] = component.material;
}


void from_json(nlohmann::json const& j, Name& component) {
    component.name = j["name"];
}

void to_json(nlohmann::json& j, Name const& component) {
    j["name"] = component.name;
}


void from_json(nlohmann::json const& j, StaticMesh& component) {
    component.mesh = j["mesh"];
}

void to_json(nlohmann::json& j, StaticMesh const& component) {
    j["mesh"] = component.mesh;
}


void from_json(nlohmann::json const& j, PointLight& component) {
    component.ambient = j["ambient"];
    component.diffuse = j["diffuse"];
    component.specular = j["specular"];
}

void to_json(nlohmann::json& j, PointLight const& component) {
    j["ambient"] = component.ambient;
    j["diffuse"] = component.diffuse;
    j["specular"] = component.specular;
}


void from_json(nlohmann::json const& j, Transform& component) {
    component.position = j["position"];
    component.rotation = j["rotation"];
    component.scale = j["scale"];
}

void to_json(nlohmann::json& j, Transform const& component) {
    j["position"] = component.position;
    j["rotation"] = component.rotation;
    j["scale"] = component.scale;
}


void from_json(nlohmann::json const& j, Rotator& component) {
    component.speed = j["speed"];
    component.axes = j["axes"];
}

void to_json(nlohmann::json& j, Rotator const& component) {
    j["speed"] = component.speed;
    j["axes"] = component.axes;
}


void from_json(nlohmann::json const& j, EditorCamera& component) {
    component.sensitivity = j["sensitivity"];
}

void to_json(nlohmann::json& j, EditorCamera const& component) {
    j["sensitivity"] = component.sensitivity;
}



}

namespace saturn::ecs {

static void do_deserialize(registry& ecs, nlohmann::json const& j, entity_t entity) {
    using namespace components;
    // Blueprint instances are handled in the blueprint resolve pass
    if (auto json_it = j.find("BlueprintInstance"); json_it != j.end()) { 
        ecs.add_component<BlueprintInstance>(entity);
        ecs.get_component<BlueprintInstance>(entity) = *json_it;
    
        return;
    }

    if (auto json_it = j.find("Blueprint"); json_it != j.end()) {
        if (!(*json_it)["model"].is_null()) {
            serialize_context->scene->models_to_load.push_back({entity, fs::path((*json_it)["model"].get<std::string>())});
        }
    }

    if (auto json_it = j.find("Blueprint"); json_it != j.end()) {
        ecs.add_component<Blueprint>(entity);
        ecs.get_component<Blueprint>(entity) = *json_it;
    }
    if (auto json_it = j.find("BlueprintInstance"); json_it != j.end()) {
        ecs.add_component<BlueprintInstance>(entity);
        ecs.get_component<BlueprintInstance>(entity) = *json_it;
    }
    if (auto json_it = j.find("Camera"); json_it != j.end()) {
        ecs.add_component<Camera>(entity);
        ecs.get_component<Camera>(entity) = *json_it;
    }
    if (auto json_it = j.find("MeshRenderer"); json_it != j.end()) {
        ecs.add_component<MeshRenderer>(entity);
        ecs.get_component<MeshRenderer>(entity) = *json_it;
    }
    if (auto json_it = j.find("Name"); json_it != j.end()) {
        ecs.add_component<Name>(entity);
        ecs.get_component<Name>(entity) = *json_it;
    }
    if (auto json_it = j.find("StaticMesh"); json_it != j.end()) {
        ecs.add_component<StaticMesh>(entity);
        ecs.get_component<StaticMesh>(entity) = *json_it;
    }
    if (auto json_it = j.find("PointLight"); json_it != j.end()) {
        ecs.add_component<PointLight>(entity);
        ecs.get_component<PointLight>(entity) = *json_it;
    }
    if (auto json_it = j.find("Transform"); json_it != j.end()) {
        ecs.add_component<Transform>(entity);
        ecs.get_component<Transform>(entity) = *json_it;
    }
    if (auto json_it = j.find("Rotator"); json_it != j.end()) {
        ecs.add_component<Rotator>(entity);
        ecs.get_component<Rotator>(entity) = *json_it;
    }
    if (auto json_it = j.find("EditorCamera"); json_it != j.end()) {
        ecs.add_component<EditorCamera>(entity);
        ecs.get_component<EditorCamera>(entity) = *json_it;
    }
}

void deserialize_into_entity(nlohmann::json const& j, registry& ecs, entity_t entity) {
    do_deserialize(ecs, j, entity);
    auto children = j.find("Children");
    if (children != j.end()) {
        for (auto const& child : *children) {
            entity_t child_entity = ecs.create_entity(entity);
            deserialize_into_entity(child, ecs, child_entity);
        }
    }
}

static void do_serialize(registry const& ecs, nlohmann::json& j, entity_t entity) {
    using namespace components;
    // Blueprint instances are handled in a special way
    if (ecs.has_component<components::BlueprintInstance>(entity)) {
        auto const& bp = ecs.get_component<components::BlueprintInstance>(entity);
        j["BlueprintInstance"]["blueprint"] = bp.blueprint;
        return;
    }
    if (ecs.has_component<Blueprint>(entity)) {
        j["Blueprint"] = ecs.get_component<Blueprint>(entity);
    }
    if (ecs.has_component<BlueprintInstance>(entity)) {
        j["BlueprintInstance"] = ecs.get_component<BlueprintInstance>(entity);
    }
    if (ecs.has_component<Camera>(entity)) {
        j["Camera"] = ecs.get_component<Camera>(entity);
    }
    if (ecs.has_component<MeshRenderer>(entity)) {
        j["MeshRenderer"] = ecs.get_component<MeshRenderer>(entity);
    }
    if (ecs.has_component<Name>(entity)) {
        j["Name"] = ecs.get_component<Name>(entity);
    }
    if (ecs.has_component<StaticMesh>(entity)) {
        j["StaticMesh"] = ecs.get_component<StaticMesh>(entity);
    }
    if (ecs.has_component<PointLight>(entity)) {
        j["PointLight"] = ecs.get_component<PointLight>(entity);
    }
    if (ecs.has_component<Transform>(entity)) {
        j["Transform"] = ecs.get_component<Transform>(entity);
    }
    if (ecs.has_component<Rotator>(entity)) {
        j["Rotator"] = ecs.get_component<Rotator>(entity);
    }
    if (ecs.has_component<EditorCamera>(entity)) {
        j["EditorCamera"] = ecs.get_component<EditorCamera>(entity);
    }
}

void serialize_from_entity(nlohmann::json& j, registry const& ecs, entity_t entity) {
    auto entity_it = ecs.get_entities().find(entity);

    auto serialize_fun = [&ecs](entity_t entity, stl::tree<entity_t>::const_traverse_info const& info, nlohmann::json& j)
        -> stl::tuple<nlohmann::json&> {
        
        nlohmann::json entity_j;
        entity_j["Children"] = nlohmann::json::array();
        do_serialize(ecs, entity_j, entity);
        j.push_back(stl::move(entity_j));
        return stl::tie(j.back()["Children"]);
    };
    
   ecs.get_entities().traverse_from(entity_it, serialize_fun, j);
}


void from_json(nlohmann::json const& j, registry& ecs) {
    for (auto const& json_entity : j) {
        auto ent = ecs.create_entity();
        deserialize_into_entity(json_entity, ecs, ent);
    }
}

void to_json(nlohmann::json& j, registry const& ecs) {
    j = nlohmann::json::array();

    serialize_from_entity(j, ecs, ecs.get_entities().root()->data);
}

}