#include <saturn/serialization/component_serializers.hpp>
#include <saturn/serialization/default_serializers.hpp>

#include <phobos/renderer/material.hpp>
#include <phobos/renderer/mesh.hpp>
#include <phobos/renderer/texture.hpp>

namespace saturn::components {

void from_json(nlohmann::json const& j, Material& component) {
    component.texture = j["texture"];
}

void to_json(nlohmann::json& j, Material const& component) {
    j["texture"] = component.texture;
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
void from_json(nlohmann::json const& j, StaticMesh& component) {
    component.mesh = j["mesh"];
}

void to_json(nlohmann::json& j, StaticMesh const& component) {
    j["mesh"] = component.mesh;
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

}

namespace saturn::ecs {

static void deserialize_into_entity(nlohmann::json const& j, registry& ecs, entity_t entity) {
    using namespace components;
    if (auto json_it = j.find("Material"); json_it != j.end()) {
        ecs.add_component<Material>(entity);
        ecs.get_component<Material>(entity) = *json_it;
    }
    if (auto json_it = j.find("PointLight"); json_it != j.end()) {
        ecs.add_component<PointLight>(entity);
        ecs.get_component<PointLight>(entity) = *json_it;
    }
    if (auto json_it = j.find("StaticMesh"); json_it != j.end()) {
        ecs.add_component<StaticMesh>(entity);
        ecs.get_component<StaticMesh>(entity) = *json_it;
    }
    if (auto json_it = j.find("Transform"); json_it != j.end()) {
        ecs.add_component<Transform>(entity);
        ecs.get_component<Transform>(entity) = *json_it;
    }
}

static void serialize_from_entity(nlohmann::json& j, registry const& ecs, entity_t entity) {
    using namespace components;
    if (ecs.has_component<Material>(entity)) {
        j["Material"] = ecs.get_component<Material>(entity);
    }
    if (ecs.has_component<PointLight>(entity)) {
        j["PointLight"] = ecs.get_component<PointLight>(entity);
    }
    if (ecs.has_component<StaticMesh>(entity)) {
        j["StaticMesh"] = ecs.get_component<StaticMesh>(entity);
    }
    if (ecs.has_component<Transform>(entity)) {
        j["Transform"] = ecs.get_component<Transform>(entity);
    }
}

void from_json(nlohmann::json const& j, registry& ecs) {
    for (auto const& json_entity : j) {
        auto ent = ecs.create_entity();
        deserialize_into_entity(json_entity, ecs, ent);
    }
}

void to_json(nlohmann::json& j, registry const& ecs) {
    j = nlohmann::json::array();
    for (entity_t ent : ecs.get_entities()) {
        nlohmann::json& ent_json = j.emplace_back(nlohmann::json::object());
        serialize_from_entity(ent_json, ecs, ent);
    }
}

}