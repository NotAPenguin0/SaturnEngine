// The from_json function in this file was generated code.
// When modifying other code, make sure to edit the template as well.
// At some point, I will move this generated function out of this file
// to avoid this problem. #TODO

#include "Scene/SceneObject.hpp"
#include "Scene/Scene.hpp"

#include "Core/Application.hpp"
#include "AssetManager/AssetManager.hpp"
#include "ECS/ComponentList.hpp"
#include "ECS/Components.hpp"
#include "Math/Math.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Viewport.hpp"
#include "Serialization/ComponentSerializers.hpp"
#include "Serialization/CustomSerializers.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

namespace Saturn {

SceneObject::SceneObject(Scene* s, SceneObject* parent /*= nullptr*/) :
    scene(s), parent_obj(parent) {
    if (has_parent()) { parent_id = parent_obj->get_id(); }
}

bool SceneObject::has_parent() const { return parent_obj != nullptr; }

SceneObject* SceneObject::parent() { return parent_obj; }

SceneObject const* SceneObject::parent() const { return parent_obj; }

void SceneObject::serialize_to_file(std::string_view path) {
    nlohmann::json json;
    json = *this;
    std::ofstream file(path.data());
    file << json.dump(4);
}

void to_json(nlohmann::json& j, SceneObject const& obj) {
    obj.serialize_components<COMPONENT_LIST>(j);
    j["ID"] = obj.get_id();
    j["ParentID"] = obj.get_parent_id();
}


void from_json(nlohmann::json const& j, SceneObject& obj) {
    using namespace Components;
    auto& ecs = obj.get_scene()->get_ecs();

	// Deserialization for BoxCollider component
	if (auto const& component = j.find("BoxColliderComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<BoxCollider>(
			obj.add_component<BoxCollider>());
		j.get_to(c);
		
	}
	// Deserialization for Button component
	if (auto const& component = j.find("ButtonComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Button>(
			obj.add_component<Button>());
		j.get_to(c);
		
	}
	// Deserialization for Camera component
	if (auto const& component = j.find("CameraComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Camera>(
			obj.add_component<Camera>());
		j.get_to(c);
		
	}
	// Deserialization for CameraZoomController component
	if (auto const& component = j.find("CameraZoomControllerComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<CameraZoomController>(
			obj.add_component<CameraZoomController>());
		j.get_to(c);
		
	}
	// Deserialization for Canvas component
	if (auto const& component = j.find("CanvasComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Canvas>(
			obj.add_component<Canvas>());
		j.get_to(c);
		
	}
	// Deserialization for ClickEffect component
	if (auto const& component = j.find("ClickEffectComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<ClickEffect>(
			obj.add_component<ClickEffect>());
		j.get_to(c);
		
	}
	// Deserialization for ColliderRenderer component
	if (auto const& component = j.find("ColliderRendererComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<ColliderRenderer>(
			obj.add_component<ColliderRenderer>());
		j.get_to(c);
		
	}
	// Deserialization for DirectionalLight component
	if (auto const& component = j.find("DirectionalLightComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<DirectionalLight>(
			obj.add_component<DirectionalLight>());
		j.get_to(c);
		
	}
	// Deserialization for DoNotSerialize component
	if (auto const& component = j.find("DoNotSerializeComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<DoNotSerialize>(
			obj.add_component<DoNotSerialize>());
		j.get_to(c);
		
	}
	// Deserialization for EditorCameraController component
	if (auto const& component = j.find("EditorCameraControllerComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<EditorCameraController>(
			obj.add_component<EditorCameraController>());
		j.get_to(c);
		
	}
	// Deserialization for FPSCameraController component
	if (auto const& component = j.find("FPSCameraControllerComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<FPSCameraController>(
			obj.add_component<FPSCameraController>());
		j.get_to(c);
		
	}
	// Deserialization for FreeLookController component
	if (auto const& component = j.find("FreeLookControllerComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<FreeLookController>(
			obj.add_component<FreeLookController>());
		j.get_to(c);
		
	}
	// Deserialization for Image component
	if (auto const& component = j.find("ImageComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Image>(
			obj.add_component<Image>());
		j.get_to(c);
		
	}
	// Deserialization for MusicController component
	if (auto const& component = j.find("MusicControllerComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<MusicController>(
			obj.add_component<MusicController>());
		j.get_to(c);
		
	}
	// Deserialization for Name component
	if (auto const& component = j.find("NameComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Name>(
			obj.add_component<Name>());
		j.get_to(c);
		
	}
	// Deserialization for OutlineRenderer component
	if (auto const& component = j.find("OutlineRendererComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<OutlineRenderer>(
			obj.add_component<OutlineRenderer>());
		j.get_to(c);
		
	}
	// Deserialization for ParticleEmitter component
	if (auto const& component = j.find("ParticleEmitterComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<ParticleEmitter>(
			obj.add_component<ParticleEmitter>());
		j.get_to(c);
		
	}
	// Deserialization for PointLight component
	if (auto const& component = j.find("PointLightComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<PointLight>(
			obj.add_component<PointLight>());
		j.get_to(c);
		
	}
	// Deserialization for Rigidbody component
	if (auto const& component = j.find("RigidbodyComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Rigidbody>(
			obj.add_component<Rigidbody>());
		j.get_to(c);
		
	}
	// Deserialization for Rotator component
	if (auto const& component = j.find("RotatorComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Rotator>(
			obj.add_component<Rotator>());
		j.get_to(c);
		
	}
	// Deserialization for Sound component
	if (auto const& component = j.find("SoundComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Sound>(
			obj.add_component<Sound>());
		j.get_to(c);
		
	}
	// Deserialization for SoundListener component
	if (auto const& component = j.find("SoundListenerComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<SoundListener>(
			obj.add_component<SoundListener>());
		j.get_to(c);
		
	}
	// Deserialization for SpotLight component
	if (auto const& component = j.find("SpotLightComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<SpotLight>(
			obj.add_component<SpotLight>());
		j.get_to(c);
		
	}
	// Deserialization for StaticMesh component
	if (auto const& component = j.find("StaticMeshComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<StaticMesh>(
			obj.add_component<StaticMesh>());
		j.get_to(c);
		
	}
	// Deserialization for StaticModel component
	if (auto const& component = j.find("StaticModelComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<StaticModel>(
			obj.add_component<StaticModel>());
		j.get_to(c);
		
	}
	// Deserialization for Text component
	if (auto const& component = j.find("TextComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Text>(
			obj.add_component<Text>());
		j.get_to(c);
		
	}
	// Deserialization for Transform component
	if (auto const& component = j.find("TransformComponent"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<Transform>(
			obj.add_component<Transform>());
		j.get_to(c);
		
	}
	
	
    // Deserialize parent entity
    if (auto const& parent = j.find("ParentID"); parent != j.end()) {
        std::size_t p_id = (*parent).get<int>();
        obj.set_parent_id(p_id);
    }

    if (auto const& id = j.find("ID"); id != j.end()) {
        obj.set_id((*id).get<int>());
    }
} 

} // namespace Saturn
