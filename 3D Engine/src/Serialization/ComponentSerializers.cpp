/*	This file was generated using the serialization code generation program 
 *	for SaturnEngine. Do NOT modify this file manually.
 */
 
#include "Serialization/ComponentSerializers.hpp"
#include "Serialization/CustomSerializers.hpp"
#include "AssetManager/AssetManager.hpp"
#include "ECS/Components.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace Saturn::Components {

// Deserialization functions

void from_json(nlohmann::json const& json, BoxCollider& component) {
	auto c = json.find("BoxColliderComponent");
	if (c == json.end()) {
		throw std::runtime_error("No BoxCollider component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Center"); x != (*c).end()) {
			component.center = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("HalfWidths"); x != (*c).end()) {
			component.half_widths = (*x).get<glm::vec3>();
		}
	}
}

void from_json(nlohmann::json const& json, Button& component) {
	auto c = json.find("ButtonComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Button component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Position"); x != (*c).end()) {
			component.position = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Image"); x != (*c).end()) {
			component.image = (*x).get<Resource<Saturn::Texture>>();
		}
		if (auto x = (*c).find("TextColor"); x != (*c).end()) {
			component.text_color = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Size"); x != (*c).end()) {
			component.size = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Anchor"); x != (*c).end()) {
			component.anchor = (*x).get<ui_anchors::anchor_t>();
		}
	}
}

void from_json(nlohmann::json const& json, Camera& component) {
	auto c = json.find("CameraComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Camera component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Up"); x != (*c).end()) {
			component.up = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("Front"); x != (*c).end()) {
			component.front = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("Fov"); x != (*c).end()) {
			component.fov = (*x).get<float>();
		}
		if (auto x = (*c).find("ViewportId"); x != (*c).end()) {
			component.viewport_id = (*x).get<unsigned int>();
		}
	}
}

void from_json(nlohmann::json const& json, CameraZoomController& component) {
	auto c = json.find("CameraZoomControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No CameraZoomController component stored even though it was requested");
	} else {
		if (auto x = (*c).find("ZoomSpeed"); x != (*c).end()) {
			component.zoom_speed = (*x).get<float>();
		}
		if (auto x = (*c).find("MinZoom"); x != (*c).end()) {
			component.min_zoom = (*x).get<float>();
		}
		if (auto x = (*c).find("MaxZoom"); x != (*c).end()) {
			component.max_zoom = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, Canvas& component) {
	auto c = json.find("CanvasComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Canvas component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Position"); x != (*c).end()) {
			component.position = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Size"); x != (*c).end()) {
			component.size = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Opacity"); x != (*c).end()) {
			component.opacity = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, ClickEffect& component) {
	auto c = json.find("ClickEffectComponent");
	if (c == json.end()) {
		throw std::runtime_error("No ClickEffect component stored even though it was requested");
	} else {
		if (auto x = (*c).find("SoundEffect"); x != (*c).end()) {
			component.sound_effect = (*x).get<Resource<audeo::SoundSource>>();
		}
		if (auto x = (*c).find("MaxDistance"); x != (*c).end()) {
			component.max_distance = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, ColliderRenderer& component) {
	auto c = json.find("ColliderRendererComponent");
	if (c == json.end()) {
		throw std::runtime_error("No ColliderRenderer component stored even though it was requested");
	} else {
		
	}
}

void from_json(nlohmann::json const& json, DirectionalLight& component) {
	auto c = json.find("DirectionalLightComponent");
	if (c == json.end()) {
		throw std::runtime_error("No DirectionalLight component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Ambient"); x != (*c).end()) {
			component.ambient = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Diffuse"); x != (*c).end()) {
			component.diffuse = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Specular"); x != (*c).end()) {
			component.specular = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Direction"); x != (*c).end()) {
			component.direction = (*x).get<glm::vec3>();
		}
	}
}

void from_json(nlohmann::json const& json, EditorCameraController& component) {
	auto c = json.find("EditorCameraControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No EditorCameraController component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Speed"); x != (*c).end()) {
			component.speed = (*x).get<float>();
		}
		if (auto x = (*c).find("Sensitivity"); x != (*c).end()) {
			component.sensitivity = (*x).get<float>();
		}
		if (auto x = (*c).find("ZoomSpeed"); x != (*c).end()) {
			component.zoom_speed = (*x).get<float>();
		}
		if (auto x = (*c).find("InvertX"); x != (*c).end()) {
			component.invert_x = (*x).get<bool>();
		}
		if (auto x = (*c).find("InvertY"); x != (*c).end()) {
			component.invert_y = (*x).get<bool>();
		}
	}
}

void from_json(nlohmann::json const& json, FPSCameraController& component) {
	auto c = json.find("FPSCameraControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No FPSCameraController component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Speed"); x != (*c).end()) {
			component.speed = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, FreeLookController& component) {
	auto c = json.find("FreeLookControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No FreeLookController component stored even though it was requested");
	} else {
		if (auto x = (*c).find("MouseSensitivity"); x != (*c).end()) {
			component.mouse_sensitivity = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, Image& component) {
	auto c = json.find("ImageComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Image component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Image"); x != (*c).end()) {
			component.image = (*x).get<Resource<Saturn::Texture>>();
		}
		if (auto x = (*c).find("Position"); x != (*c).end()) {
			component.position = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Anchor"); x != (*c).end()) {
			component.anchor = (*x).get<ui_anchors::anchor_t>();
		}
		if (auto x = (*c).find("Size"); x != (*c).end()) {
			component.size = (*x).get<glm::vec2>();
		}
	}
}

void from_json(nlohmann::json const& json, Material& component) {
	auto c = json.find("MaterialComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Material component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Shader"); x != (*c).end()) {
			component.shader = (*x).get<Resource<Saturn::Shader>>();
		}
		if (auto x = (*c).find("DiffuseMap"); x != (*c).end()) {
			component.diffuse_map = (*x).get<Resource<Saturn::Texture>>();
		}
		if (auto x = (*c).find("Lit"); x != (*c).end()) {
			component.lit = (*x).get<bool>();
		}
		if (auto x = (*c).find("SpecularMap"); x != (*c).end()) {
			component.specular_map = (*x).get<Resource<Saturn::Texture>>();
		}
		if (auto x = (*c).find("NormalMap"); x != (*c).end()) {
			component.normal_map = (*x).get<Resource<Saturn::Texture>>();
		}
		if (auto x = (*c).find("Shininess"); x != (*c).end()) {
			component.shininess = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, MusicController& component) {
	auto c = json.find("MusicControllerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No MusicController component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Music"); x != (*c).end()) {
			component.music = (*x).get<Resource<audeo::SoundSource>>();
		}
		if (auto x = (*c).find("Loop"); x != (*c).end()) {
			component.loop = (*x).get<bool>();
		}
		if (auto x = (*c).find("FadeInMs"); x != (*c).end()) {
			component.fade_in_ms = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, Name& component) {
	auto c = json.find("NameComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Name component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Name"); x != (*c).end()) {
			component.name = (*x).get<std::string>();
		}
	}
}

void from_json(nlohmann::json const& json, OutlineRenderer& component) {
	auto c = json.find("OutlineRendererComponent");
	if (c == json.end()) {
		throw std::runtime_error("No OutlineRenderer component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Color"); x != (*c).end()) {
			component.color = (*x).get<Saturn::color3>();
		}
	}
}

void from_json(nlohmann::json const& json, PointLight& component) {
	auto c = json.find("PointLightComponent");
	if (c == json.end()) {
		throw std::runtime_error("No PointLight component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Ambient"); x != (*c).end()) {
			component.ambient = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Diffuse"); x != (*c).end()) {
			component.diffuse = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Specular"); x != (*c).end()) {
			component.specular = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Intensity"); x != (*c).end()) {
			component.intensity = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, Rigidbody& component) {
	auto c = json.find("RigidbodyComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Rigidbody component stored even though it was requested");
	} else {
		if (auto x = (*c).find("LockedAxes"); x != (*c).end()) {
			component.locked_axes = (*x).get<glm::bvec3>();
		}
		if (auto x = (*c).find("Mass"); x != (*c).end()) {
			component.mass = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, Rotator& component) {
	auto c = json.find("RotatorComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Rotator component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Speed"); x != (*c).end()) {
			component.speed = (*x).get<float>();
		}
		if (auto x = (*c).find("EulerAngles"); x != (*c).end()) {
			component.euler_angles = (*x).get<glm::vec3>();
		}
	}
}

void from_json(nlohmann::json const& json, SoundListener& component) {
	auto c = json.find("SoundListenerComponent");
	if (c == json.end()) {
		throw std::runtime_error("No SoundListener component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Forward"); x != (*c).end()) {
			component.forward = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("Position"); x != (*c).end()) {
			component.position = (*x).get<glm::vec3>();
		}
	}
}

void from_json(nlohmann::json const& json, SpotLight& component) {
	auto c = json.find("SpotLightComponent");
	if (c == json.end()) {
		throw std::runtime_error("No SpotLight component stored even though it was requested");
	} else {
		if (auto x = (*c).find("InnerAngle"); x != (*c).end()) {
			component.inner_angle = (*x).get<float>();
		}
		if (auto x = (*c).find("Ambient"); x != (*c).end()) {
			component.ambient = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Diffuse"); x != (*c).end()) {
			component.diffuse = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Specular"); x != (*c).end()) {
			component.specular = (*x).get<Saturn::color3>();
		}
		if (auto x = (*c).find("Intensity"); x != (*c).end()) {
			component.intensity = (*x).get<float>();
		}
		if (auto x = (*c).find("Direction"); x != (*c).end()) {
			component.direction = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("OuterAngle"); x != (*c).end()) {
			component.outer_angle = (*x).get<float>();
		}
	}
}

void from_json(nlohmann::json const& json, StaticMesh& component) {
	auto c = json.find("StaticMeshComponent");
	if (c == json.end()) {
		throw std::runtime_error("No StaticMesh component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Mesh"); x != (*c).end()) {
			component.mesh = (*x).get<Resource<Saturn::Mesh>>();
		}
		if (auto x = (*c).find("FaceCull"); x != (*c).end()) {
			component.face_cull = (*x).get<bool>();
		}
	}
}

void from_json(nlohmann::json const& json, Text& component) {
	auto c = json.find("TextComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Text component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Text"); x != (*c).end()) {
			component.text = (*x).get<std::string>();
		}
		if (auto x = (*c).find("Position"); x != (*c).end()) {
			component.position = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Anchor"); x != (*c).end()) {
			component.anchor = (*x).get<ui_anchors::anchor_t>();
		}
		if (auto x = (*c).find("Size"); x != (*c).end()) {
			component.size = (*x).get<glm::vec2>();
		}
		if (auto x = (*c).find("Font"); x != (*c).end()) {
			component.font = (*x).get<Resource<Saturn::Font>>();
		}
		if (auto x = (*c).find("Color"); x != (*c).end()) {
			component.color = (*x).get<Saturn::color3>();
		}
	}
}

void from_json(nlohmann::json const& json, Transform& component) {
	auto c = json.find("TransformComponent");
	if (c == json.end()) {
		throw std::runtime_error("No Transform component stored even though it was requested");
	} else {
		if (auto x = (*c).find("Position"); x != (*c).end()) {
			component.position = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("Rotation"); x != (*c).end()) {
			component.rotation = (*x).get<glm::vec3>();
		}
		if (auto x = (*c).find("Scale"); x != (*c).end()) {
			component.scale = (*x).get<glm::vec3>();
		}
	}
}


void to_json(nlohmann::json& json, BoxCollider const& component) {
	json["BoxColliderComponent"] = nlohmann::json::object();
	// clang-format off
	json["BoxColliderComponent"]["Center"] = component.center;
	json["BoxColliderComponent"]["HalfWidths"] = component.half_widths;
	// clang-format on
}

void to_json(nlohmann::json& json, Button const& component) {
	json["ButtonComponent"] = nlohmann::json::object();
	// clang-format off
	json["ButtonComponent"]["LastEvent"] = component.last_event;
	json["ButtonComponent"]["Position"] = component.position;
	json["ButtonComponent"]["Image"] = component.image;
	json["ButtonComponent"]["TextColor"] = component.text_color;
	json["ButtonComponent"]["Size"] = component.size;
	json["ButtonComponent"]["Anchor"] = component.anchor;
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

void to_json(nlohmann::json& json, Canvas const& component) {
	json["CanvasComponent"] = nlohmann::json::object();
	// clang-format off
	json["CanvasComponent"]["Position"] = component.position;
	json["CanvasComponent"]["Size"] = component.size;
	json["CanvasComponent"]["Opacity"] = component.opacity;
	// clang-format on
}

void to_json(nlohmann::json& json, ClickEffect const& component) {
	json["ClickEffectComponent"] = nlohmann::json::object();
	// clang-format off
	json["ClickEffectComponent"]["SoundEffect"] = component.sound_effect;
	json["ClickEffectComponent"]["MaxDistance"] = component.max_distance;
	// clang-format on
}

void to_json(nlohmann::json& json, ColliderRenderer const& component) {
	json["ColliderRendererComponent"] = nlohmann::json::object();
	// clang-format off
	
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
	json["EditorCameraControllerComponent"]["InvertX"] = component.invert_x;
	json["EditorCameraControllerComponent"]["InvertY"] = component.invert_y;
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

void to_json(nlohmann::json& json, Image const& component) {
	json["ImageComponent"] = nlohmann::json::object();
	// clang-format off
	json["ImageComponent"]["Image"] = component.image;
	json["ImageComponent"]["Position"] = component.position;
	json["ImageComponent"]["Anchor"] = component.anchor;
	json["ImageComponent"]["Size"] = component.size;
	// clang-format on
}

void to_json(nlohmann::json& json, Material const& component) {
	json["MaterialComponent"] = nlohmann::json::object();
	// clang-format off
	json["MaterialComponent"]["Shader"] = component.shader;
	json["MaterialComponent"]["DiffuseMap"] = component.diffuse_map;
	json["MaterialComponent"]["Lit"] = component.lit;
	json["MaterialComponent"]["SpecularMap"] = component.specular_map;
	json["MaterialComponent"]["NormalMap"] = component.normal_map;
	json["MaterialComponent"]["Shininess"] = component.shininess;
	// clang-format on
}

void to_json(nlohmann::json& json, MusicController const& component) {
	json["MusicControllerComponent"] = nlohmann::json::object();
	// clang-format off
	json["MusicControllerComponent"]["Music"] = component.music;
	json["MusicControllerComponent"]["Loop"] = component.loop;
	json["MusicControllerComponent"]["FadeInMs"] = component.fade_in_ms;
	// clang-format on
}

void to_json(nlohmann::json& json, Name const& component) {
	json["NameComponent"] = nlohmann::json::object();
	// clang-format off
	json["NameComponent"]["Name"] = component.name;
	// clang-format on
}

void to_json(nlohmann::json& json, OutlineRenderer const& component) {
	json["OutlineRendererComponent"] = nlohmann::json::object();
	// clang-format off
	json["OutlineRendererComponent"]["Color"] = component.color;
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
	json["RigidbodyComponent"]["LockedAxes"] = component.locked_axes;
	json["RigidbodyComponent"]["Mass"] = component.mass;
	// clang-format on
}

void to_json(nlohmann::json& json, Rotator const& component) {
	json["RotatorComponent"] = nlohmann::json::object();
	// clang-format off
	json["RotatorComponent"]["Speed"] = component.speed;
	json["RotatorComponent"]["EulerAngles"] = component.euler_angles;
	// clang-format on
}

void to_json(nlohmann::json& json, SoundListener const& component) {
	json["SoundListenerComponent"] = nlohmann::json::object();
	// clang-format off
	json["SoundListenerComponent"]["Forward"] = component.forward;
	json["SoundListenerComponent"]["Position"] = component.position;
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

void to_json(nlohmann::json& json, Text const& component) {
	json["TextComponent"] = nlohmann::json::object();
	// clang-format off
	json["TextComponent"]["Text"] = component.text;
	json["TextComponent"]["Position"] = component.position;
	json["TextComponent"]["Anchor"] = component.anchor;
	json["TextComponent"]["Size"] = component.size;
	json["TextComponent"]["Font"] = component.font;
	json["TextComponent"]["Color"] = component.color;
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