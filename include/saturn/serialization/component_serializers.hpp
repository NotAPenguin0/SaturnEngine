#ifndef SATURN_COMPONENT_SERIALIZERS_HPP_
#define SATURN_COMPONENT_SERIALIZERS_HPP_

#include <nlohmann/json.hpp>

#include <saturn/components/blueprint.hpp>
#include <saturn/components/blueprint_instance.hpp>
#include <saturn/components/camera.hpp>
#include <saturn/components/mesh_renderer.hpp>
#include <saturn/components/name.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/point_light.hpp>
#include <samples/components/rotator.hpp>
#include <editor/components/editor_camera.hpp>
#include <saturn/components/transform.hpp>

#include <saturn/ecs/registry.hpp>

namespace saturn::components {

void from_json(nlohmann::json const& j, Blueprint& component);
void to_json(nlohmann::json& j, Blueprint const& component);

void from_json(nlohmann::json const& j, BlueprintInstance& component);
void to_json(nlohmann::json& j, BlueprintInstance const& component);

void from_json(nlohmann::json const& j, Camera& component);
void to_json(nlohmann::json& j, Camera const& component);

void from_json(nlohmann::json const& j, MeshRenderer& component);
void to_json(nlohmann::json& j, MeshRenderer const& component);

void from_json(nlohmann::json const& j, Name& component);
void to_json(nlohmann::json& j, Name const& component);

void from_json(nlohmann::json const& j, StaticMesh& component);
void to_json(nlohmann::json& j, StaticMesh const& component);

void from_json(nlohmann::json const& j, PointLight& component);
void to_json(nlohmann::json& j, PointLight const& component);

void from_json(nlohmann::json const& j, Rotator& component);
void to_json(nlohmann::json& j, Rotator const& component);

void from_json(nlohmann::json const& j, EditorCamera& component);
void to_json(nlohmann::json& j, EditorCamera const& component);

void from_json(nlohmann::json const& j, Transform& component);
void to_json(nlohmann::json& j, Transform const& component);


}

namespace saturn::ecs {

void deserialize_into_entity(nlohmann::json const& j, registry& ecs, entity_t entity);
void serialize_from_entity(nlohmann::json& j, registry const& ecs, entity_t entity);

void from_json(nlohmann::json const& j, registry& ecs);
void to_json(nlohmann::json& j, registry const& ecs);

}

#endif