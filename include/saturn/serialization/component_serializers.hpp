#ifndef SATURN_COMPONENT_SERIALIZERS_HPP_
#define SATURN_COMPONENT_SERIALIZERS_HPP_

#include <nlohmann/json.hpp>

#include <saturn/components/mesh_renderer.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/point_light.hpp>
#include <saturn/components/transform.hpp>
#include <samples/components/rotator.hpp>

#include <saturn/ecs/registry.hpp>

namespace saturn::components {

void from_json(nlohmann::json const& j, MeshRenderer& component);
void to_json(nlohmann::json& j, MeshRenderer const& component);

void from_json(nlohmann::json const& j, StaticMesh& component);
void to_json(nlohmann::json& j, StaticMesh const& component);

void from_json(nlohmann::json const& j, PointLight& component);
void to_json(nlohmann::json& j, PointLight const& component);

void from_json(nlohmann::json const& j, Transform& component);
void to_json(nlohmann::json& j, Transform const& component);

void from_json(nlohmann::json const& j, Rotator& component);
void to_json(nlohmann::json& j, Rotator const& component);


}

namespace saturn::ecs {

void deserialize_into_entity(nlohmann::json const& j, registry& ecs, entity_t entity);
void serialize_from_entity(nlohmann::json& j, registry const& ecs, entity_t entity);

void from_json(nlohmann::json const& j, registry& ecs);
void to_json(nlohmann::json& j, registry const& ecs);

}

#endif