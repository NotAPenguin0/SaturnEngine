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