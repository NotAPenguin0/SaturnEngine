/*	This file was generated using the serialization code generation program 
 *	for SaturnEngine. Do NOT modify this file manually.
 */
 
#include "Subsystems/Serialization/ComponentSerializers.hpp"
#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/ECS/Components.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace Saturn::Components {

// Deserialization functions

void from_json(nlohmann::json const& json, BoxCollider& component) {
	auto c = json.find("BoxColliderComponent");
	if (c == json.end()) {
		throw std::runtime_error("No BoxCollider component stored even though it was requested");
	} else {
		component.center = (*c)["Center"].get<glm::vec3>();
		component.half_widths = (*c)["HalfWidths"].get<glm::vec3>();
	}
}

void from_json(nlohmann::json const& json, Camera& component) {
	auto c = json.find("CameraComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Camera component stored even though it was requested");
	} else {
		component.up = (*c)["Up"].get<glm::vec3>();
		component.front = (*c)["Front"].get<glm::vec3>();
		component.fov = (*c)["Fov"].get<float>();
		component.viewport_id = (*c)["ViewportId"].get<unsigned int>();
	}
}

void from_json(nlohmann::json const& json, CameraZoomController& component) {
	auto c = json.find("CameraZoomControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No CameraZoomController component stored even though it was requested");
	} else {
		component.zoom_speed = (*c)["ZoomSpeed"].get<float>();
		component.min_zoom = (*c)["MinZoom"].get<float>();
		component.max_zoom = (*c)["MaxZoom"].get<float>();
	}
}

void from_json(nlohmann::json const& json, DirectionalLight& component) {
	auto c = json.find("DirectionalLightComponent");
	if (c == json.end()) {
		throw std::runtime_error("No DirectionalLight component stored even though it was requested");
	} else {
		component.ambient = (*c)["Ambient"].get<Saturn::color3>();
		component.diffuse = (*c)["Diffuse"].get<Saturn::color3>();
		component.specular = (*c)["Specular"].get<Saturn::color3>();
		component.direction = (*c)["Direction"].get<glm::vec3>();
	}
}

void from_json(nlohmann::json const& json, EditorCameraController& component) {
	auto c = json.find("EditorCameraControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No EditorCameraController component stored even though it was requested");
	} else {
		component.speed = (*c)["Speed"].get<float>();
		component.sensitivity = (*c)["Sensitivity"].get<float>();
		component.zoom_speed = (*c)["ZoomSpeed"].get<float>();
	}
}

void from_json(nlohmann::json const& json, FPSCameraController& component) {
	auto c = json.find("FPSCameraControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No FPSCameraController component stored even though it was requested");
	} else {
		component.speed = (*c)["Speed"].get<float>();
	}
}

void from_json(nlohmann::json const& json, FreeLookController& component) {
	auto c = json.find("FreeLookControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No FreeLookController component stored even though it was requested");
	} else {
		component.mouse_sensitivity = (*c)["MouseSensitivity"].get<float>();
	}
}

void from_json(nlohmann::json const& json, Material& component) {
	auto c = json.find("MaterialComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Material component stored even though it was requested");
	} else {
		component.diffuse_map = (*c)["DiffuseMap"].get<Resource<Saturn::Texture>>();
		component.shader = (*c)["Shader"].get<Resource<Saturn::Shader>>();
		component.lit = (*c)["Lit"].get<bool>();
		component.specular_map = (*c)["SpecularMap"].get<Resource<Saturn::Texture>>();
		component.shininess = (*c)["Shininess"].get<float>();
	}
}

void from_json(nlohmann::json const& json, Name& component) {
	auto c = json.find("NameComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Name component stored even though it was requested");
	} else {
		component.name = (*c)["Name"].get<std::string>();
	}
}

void from_json(nlohmann::json const& json, PointLight& component) {
	auto c = json.find("PointLightComponent");
	if (c == json.end()) {
		throw std::runtime_error("No PointLight component stored even though it was requested");
	} else {
		component.ambient = (*c)["Ambient"].get<Saturn::color3>();
		component.diffuse = (*c)["Diffuse"].get<Saturn::color3>();
		component.specular = (*c)["Specular"].get<Saturn::color3>();
		component.intensity = (*c)["Intensity"].get<float>();
	}
}

void from_json(nlohmann::json const& json, Rigidbody& component) {
	auto c = json.find("RigidbodyComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Rigidbody component stored even though it was requested");
	} else {
		component.mass = (*c)["Mass"].get<float>();
		component.locked_axes = (*c)["LockedAxes"].get<glm::bvec3>();
	}
}

void from_json(nlohmann::json const& json, Rotator& component) {
	auto c = json.find("RotatorComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Rotator component stored even though it was requested");
	} else {
		component.speed = (*c)["Speed"].get<float>();
		component.euler_angles = (*c)["EulerAngles"].get<glm::vec3>();
	}
}

void from_json(nlohmann::json const& json, SpotLight& component) {
	auto c = json.find("SpotLightComponent");
	if (c == json.end()) {
		throw std::runtime_error("No SpotLight component stored even though it was requested");
	} else {
		component.inner_angle = (*c)["InnerAngle"].get<float>();
		component.ambient = (*c)["Ambient"].get<Saturn::color3>();
		component.diffuse = (*c)["Diffuse"].get<Saturn::color3>();
		component.specular = (*c)["Specular"].get<Saturn::color3>();
		component.intensity = (*c)["Intensity"].get<float>();
		component.direction = (*c)["Direction"].get<glm::vec3>();
		component.outer_angle = (*c)["OuterAngle"].get<float>();
	}
}

void from_json(nlohmann::json const& json, StaticMesh& component) {
	auto c = json.find("StaticMeshComponent");
	if (c == json.end()) {
		throw std::runtime_error("No StaticMesh component stored even though it was requested");
	} else {
		component.mesh = (*c)["Mesh"].get<Resource<Saturn::Mesh>>();
		component.face_cull = (*c)["FaceCull"].get<bool>();
	}
}

void from_json(nlohmann::json const& json, Transform& component) {
	auto c = json.find("TransformComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Transform component stored even though it was requested");
	} else {
		component.position = (*c)["Position"].get<glm::vec3>();
		component.rotation = (*c)["Rotation"].get<glm::vec3>();
		component.scale = (*c)["Scale"].get<glm::vec3>();
	}
}


void to_json(nlohmann::json& json, BoxCollider const& component) {
	json["BoxColliderComponent"] = nlohmann::json::object();
	// clang-format off
	json["BoxColliderComponent"]["Center"] = component.center;
	json["BoxColliderComponent"]["HalfWidths"] = component.half_widths;
	// clang-format on
}

void to_json(nlohmann::json& json, Camera const& component) {
	json["CameraComponent"] = nlohmann::json::object();
	// clang-format off
	json["CameraComponent"]["Up"] = component.up;
	json["CameraComponent"]["Front"] = component.front;
	json["CameraComponent"]["Fov"] = component.fov;
	json["CameraComponent"]["ViewportId"] = component.viewport_id;
	// clang-format on
}

void to_json(nlohmann::json& json, CameraZoomController const& component) {
	json["CameraZoomControllerComponent"] = nlohmann::json::object();
	// clang-format off
	json["CameraZoomControllerComponent"]["ZoomSpeed"] = component.zoom_speed;
	json["CameraZoomControllerComponent"]["MinZoom"] = component.min_zoom;
	json["CameraZoomControllerComponent"]["MaxZoom"] = component.max_zoom;
	// clang-format on
}

void to_json(nlohmann::json& json, DirectionalLight const& component) {
	json["DirectionalLightComponent"] = nlohmann::json::object();
	// clang-format off
	json["DirectionalLightComponent"]["Ambient"] = component.ambient;
	json["DirectionalLightComponent"]["Diffuse"] = component.diffuse;
	json["DirectionalLightComponent"]["Specular"] = component.specular;
	json["DirectionalLightComponent"]["Direction"] = component.direction;
	// clang-format on
}

void to_json(nlohmann::json& json, EditorCameraController const& component) {
	json["EditorCameraControllerComponent"] = nlohmann::json::object();
	// clang-format off
	json["EditorCameraControllerComponent"]["Speed"] = component.speed;
	json["EditorCameraControllerComponent"]["Sensitivity"] = component.sensitivity;
	json["EditorCameraControllerComponent"]["ZoomSpeed"] = component.zoom_speed;
	// clang-format on
}

void to_json(nlohmann::json& json, FPSCameraController const& component) {
	json["FPSCameraControllerComponent"] = nlohmann::json::object();
	// clang-format off
	json["FPSCameraControllerComponent"]["Speed"] = component.speed;
	// clang-format on
}

void to_json(nlohmann::json& json, FreeLookController const& component) {
	json["FreeLookControllerComponent"] = nlohmann::json::object();
	// clang-format off
	json["FreeLookControllerComponent"]["MouseSensitivity"] = component.mouse_sensitivity;
	// clang-format on
}

void to_json(nlohmann::json& json, Material const& component) {
	json["MaterialComponent"] = nlohmann::json::object();
	// clang-format off
	json["MaterialComponent"]["DiffuseMap"] = component.diffuse_map;
	json["MaterialComponent"]["Shader"] = component.shader;
	json["MaterialComponent"]["Lit"] = component.lit;
	json["MaterialComponent"]["SpecularMap"] = component.specular_map;
	json["MaterialComponent"]["Shininess"] = component.shininess;
	// clang-format on
}

void to_json(nlohmann::json& json, Name const& component) {
	json["NameComponent"] = nlohmann::json::object();
	// clang-format off
	json["NameComponent"]["Name"] = component.name;
	// clang-format on
}

void to_json(nlohmann::json& json, PointLight const& component) {
	json["PointLightComponent"] = nlohmann::json::object();
	// clang-format off
	json["PointLightComponent"]["Ambient"] = component.ambient;
	json["PointLightComponent"]["Diffuse"] = component.diffuse;
	json["PointLightComponent"]["Specular"] = component.specular;
	json["PointLightComponent"]["Intensity"] = component.intensity;
	// clang-format on
}

void to_json(nlohmann::json& json, Rigidbody const& component) {
	json["RigidbodyComponent"] = nlohmann::json::object();
	// clang-format off
	json["RigidbodyComponent"]["Mass"] = component.mass;
	json["RigidbodyComponent"]["LockedAxes"] = component.locked_axes;
	// clang-format on
}

void to_json(nlohmann::json& json, Rotator const& component) {
	json["RotatorComponent"] = nlohmann::json::object();
	// clang-format off
	json["RotatorComponent"]["Speed"] = component.speed;
	json["RotatorComponent"]["EulerAngles"] = component.euler_angles;
	// clang-format on
}

void to_json(nlohmann::json& json, SpotLight const& component) {
	json["SpotLightComponent"] = nlohmann::json::object();
	// clang-format off
	json["SpotLightComponent"]["InnerAngle"] = component.inner_angle;
	json["SpotLightComponent"]["Ambient"] = component.ambient;
	json["SpotLightComponent"]["Diffuse"] = component.diffuse;
	json["SpotLightComponent"]["Specular"] = component.specular;
	json["SpotLightComponent"]["Intensity"] = component.intensity;
	json["SpotLightComponent"]["Direction"] = component.direction;
	json["SpotLightComponent"]["OuterAngle"] = component.outer_angle;
	// clang-format on
}

void to_json(nlohmann::json& json, StaticMesh const& component) {
	json["StaticMeshComponent"] = nlohmann::json::object();
	// clang-format off
	json["StaticMeshComponent"]["Mesh"] = component.mesh;
	json["StaticMeshComponent"]["FaceCull"] = component.face_cull;
	// clang-format on
}

void to_json(nlohmann::json& json, Transform const& component) {
	json["TransformComponent"] = nlohmann::json::object();
	// clang-format off
	json["TransformComponent"]["Position"] = component.position;
	json["TransformComponent"]["Rotation"] = component.rotation;
	json["TransformComponent"]["Scale"] = component.scale;
	// clang-format on
}


} // namespace Saturn::Components