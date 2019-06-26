#include "Subsystems/Serialization/ComponentSerializers.hpp"
#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/ECS/Components.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace Saturn::Components {

// Deserialization

void from_json(nlohmann::json const& json, Transform& transform) {
    auto trans = json.find("TransformComponent");
    if (trans == json.end()) {
        throw std::runtime_error(
            "No Transform component stored even though it was requested");
    } else {
        transform.position = (*trans)["Position"].get<glm::vec3>();
        transform.rotation = (*trans)["Rotation"].get<glm::vec3>();
        transform.scale = (*trans)["Scale"].get<glm::vec3>();
    }
}

void from_json(nlohmann::json const& json, Camera& camera) {
    auto cam = json.find("CameraComponent");
    if (cam == json.end()) {
        throw std::runtime_error(
            "No Camera component stored even though it was requested");
    } else {
        camera.front = (*cam)["Front"].get<glm::vec3>();
        camera.up = (*cam)["Up"].get<glm::vec3>();
        camera.fov = (*cam)["FOV"].get<float>();
    }
}

void from_json(nlohmann::json const& json, FPSCameraController& fps) {
    auto j = json.find("FPSCameraControllerComponent");
    if (j == json.end()) {
        throw std::runtime_error("No FPSCameraController component stored even "
                                 "though it was requested");
    } else {
        fps.speed = (*j)["Speed"].get<float>();
    }
}

void from_json(nlohmann::json const& json, FreeLookController& freelook) {
    auto j = json.find("FreeLookControllerComponent");
    if (j == json.end()) {
        throw std::runtime_error("No FreeLookController component stored even "
                                 "though it was requested");
    } else {
        freelook.mouse_sensitivity = (*j)["MouseSensitivity"].get<float>();
    }
}

void from_json(nlohmann::json const& json, CameraZoomController& zoom) {
    auto j = json.find("CameraZoomControllerComponent");
    if (j == json.end()) {
        throw std::runtime_error(
            "No CameraZoomController component stored even "
            "though it was requested");
    } else {
        zoom.zoom_speed = (*j)["ZoomSpeed"].get<float>();
        zoom.min_zoom = (*j)["MinZoom"].get<float>();
        zoom.max_zoom = (*j)["MaxZoom"].get<float>();
    }
}

void from_json(nlohmann::json const& json, StaticMesh& mesh) {
    auto j = json.find("StaticMeshComponent");
    if (j == json.end()) {
        throw std::runtime_error("No StaticMesh component stored even "
                                 "though it was requested");
    } else {
        mesh.mesh = j->get<Resource<Mesh>>();
    }
}

void from_json(nlohmann::json const& json, ParticleEmitter& emitter) {
    auto emit = json.find("ParticleEmitterComponent");
    if (emit == json.end()) {
        throw std::runtime_error(
            "No ParticleEmitter component stored even though it was requested");
    } else {

        // Main module
        auto& main = (*emit)["Main"];
        emitter.main.enabled = main["Enabled"].get<bool>();
        emitter.main.start_lifetime = main["StartLifetime"].get<float>();
        emitter.main.start_velocity = main["StartVelocity"].get<float>();
        emitter.main.start_color = main["StartColor"].get<glm::vec4>();
        emitter.main.start_size = main["StartSize"].get<glm::vec2>();
        emitter.main.max_particles = main["MaxParticles"].get<std::size_t>();
        emitter.particles.reserve(emitter.main.max_particles);
        emitter.particle_data.colors.reserve(emitter.main.max_particles);
        emitter.particle_data.positions.reserve(emitter.main.max_particles);
        emitter.particle_data.sizes.reserve(emitter.main.max_particles);
        emitter.main.loop = main["Loop"].get<bool>();
        emitter.main.duration = main["Duration"].get<float>();

        // Emission module
        auto& emission = (*emit)["Emission"];
        emitter.emission.enabled = emission["Enabled"].get<bool>();
        emitter.emission.spawn_rate = emission["SpawnRate"].get<float>();

        // Color over lifetime module
        auto& col = (*emit)["ColorOverLifetime"];
        emitter.color_over_lifetime.enabled = col["Enabled"].get<bool>();
        if (emitter.color_over_lifetime.enabled) {
            emitter.color_over_lifetime.gradient.start =
                col["Gradient"]["Start"].get<glm::vec4>();
            emitter.color_over_lifetime.gradient.end =
                col["Gradient"]["End"].get<glm::vec4>();
        }

        // Size over lifetime module
        auto& sol = (*emit)["SizeOverLifetime"];
        emitter.size_over_lifetime.enabled = sol["Enabled"].get<bool>();
        if (emitter.size_over_lifetime.enabled) {
            emitter.size_over_lifetime.modifier =
                sol["Modifier"].get<Math::Curve>();
        }

        // Velocity over lifetime module
        auto& vol = (*emit)["VelocityOverLifetime"];
        emitter.velocity_over_lifetime.enabled = vol["Enabled"].get<bool>();
        if (emitter.velocity_over_lifetime.enabled) {
            emitter.velocity_over_lifetime.modifier =
                sol["Modifier"].get<Math::Curve>();
        }

        // Shape module
        auto& shape = (*emit)["Shape"];
        emitter.shape.enabled = shape["Enabled"].get<bool>();
        if (emitter.shape.enabled) {
            emitter.shape.shape =
                shape["Type"].get<ParticleEmitter::SpawnShape>();
            switch (emitter.shape.shape) {
                case ParticleEmitter::SpawnShape::Sphere:
                    emitter.shape.radius = shape["Radius"].get<float>();
                    emitter.shape.arc = shape["Arc"].get<float>();
                    emitter.shape.angle = shape["Angle"].get<float>();
                    emitter.shape.randomize_direction =
                        shape["RandomizeDirection"].get<float>();
                    emitter.shape.random_position_offset =
                        shape["RandomPositionOffset"].get<float>();
                    emitter.shape.mode =
                        shape["SpawnMode"].get<ParticleEmitter::SpawnMode>();
                    emitter.shape.scale = shape["Scale"].get<glm::vec3>();
                    break;
                case ParticleEmitter::SpawnShape::Hemisphere: break;
                case ParticleEmitter::SpawnShape::Cone: break;
                case ParticleEmitter::SpawnShape::Box: break;
            }
        }

        emitter.additive = (*emit)["additive"].get<bool>();
    }
}

void from_json(nlohmann::json const& json, Material& material) {
    auto j = json.find("MaterialComponent");
    if (j == json.end()) {
        throw std::runtime_error("No StaticMesh component stored even "
                                 "though it was requested");
    } else {
        material.shader = (*j)["Shader"].get<Resource<Shader>>();
        if (j->find("Texture") != j->end()) {
            material.texture = (*j)["Texture"].get<Resource<Texture>>();
        }
    }
}

void from_json(nlohmann::json const& json, Rotator& rotator) {
    auto rot = json.find("RotatorComponent");
    if (rot == json.end()) {
        throw std::runtime_error(
            "No Rotator component stored even though it was requested");
    } else {
        rotator.euler_angles = (*rot)["EulerAngles"].get<glm::vec3>();
        rotator.speed = (*rot)["Speed"].get<float>();
    }
}

// Serialization

void to_json(nlohmann::json& json, Transform const& transform) {
    // clang-format off
    json["TransformComponent"] = nlohmann::json::object({
		{"Position", transform.position},
		{"Rotation", transform.rotation},
		{"Scale", transform.scale}
		});
    // clang-format on
}

void to_json(nlohmann::json& json, Camera const& camera) {
    // clang-format off
    json["CameraComponent"] = nlohmann::json::object({
		{"Front", camera.front},
		{"Up", camera.up},
		{"FOV", camera.fov}
		});
    // clang-format on 
}

void to_json(nlohmann::json& json, FPSCameraController const& fps) {
	// clang-format off
    json["FPSCameraControllerComponent"] = nlohmann::json::object({
		{"Speed", fps.speed}
		});
    // clang-format on 
}

void to_json(nlohmann::json& json, FreeLookController const& freelook) {
	// clang-format off
    json["FreeLookControllerComponent"] = nlohmann::json::object({
		{"MouseSensitivity", freelook.mouse_sensitivity}
		});
    // clang-format on 
}

void to_json(nlohmann::json& json, CameraZoomController const& zoom) {
	// clang-format off
    json["CameraZoomControllerComponent"] = nlohmann::json::object({
		{"ZoomSpeed", zoom.zoom_speed},
		{"MinZoom", zoom.min_zoom},
		{"MaxZoom", zoom.max_zoom}
		});
    // clang-format on 
}

void to_json(nlohmann::json& json, StaticMesh const& mesh) {
	// clang-format off
    json["StaticMeshComponent"] = nlohmann::json::object({
		{"Resource", mesh.mesh}
		});
    // clang-format on 
}

void to_json(nlohmann::json& json, ParticleEmitter const& emitter) {
	json["ParticleEmitterComponent"] = nlohmann::json::object();
	auto& comp = json["ParticleEmitterComponent"];
	comp["Main"] = nlohmann::json::object();
	comp["Emission"] = nlohmann::json::object();
	comp["ColorOverLifetime"] = nlohmann::json::object();
	comp["SizeOverLifetime"] = nlohmann::json::object();
	comp["VelocityOverLifetime"] = nlohmann::json::object();
	comp["Shape"] = nlohmann::json::object();
	comp["Additive"] = emitter.additive;

	auto& main = comp["Main"];
	auto& emission = comp["Emission"];
	auto& col = comp["ColorOverLifetime"];
	auto& sol = comp["SizeOverLifetime"];
	auto& vol = comp["VelocityOverLifetime"];
	auto& shape = comp["Shape"];

	main["Enabled"] = emitter.main.enabled;
	main["StartLifetime"] = emitter.main.start_lifetime;
	main["StartVelocity"] = emitter.main.start_velocity;
	main["StartColor"] = emitter.main.start_color;
	main["StartSize"] = emitter.main.start_size;
	main["MaxParticles"] = emitter.main.max_particles;
	main["Loop"] = emitter.main.loop;
	main["Duration"] = emitter.main.duration;
	
	emission["Enabled"] = emitter.emission.enabled;
	emission["SpawnRate"] = emitter.emission.spawn_rate;

	col["Enabled"] = emitter.color_over_lifetime.enabled;
	col["Gradient"]["Start"] = emitter.color_over_lifetime.gradient.start;
	col["Gradient"]["End"] = emitter.color_over_lifetime.gradient.end;

	sol["Enabled"] = emitter.size_over_lifetime.enabled;
	sol["Modifier"] = emitter.size_over_lifetime.modifier;

	vol["Enabled"] = emitter.velocity_over_lifetime.enabled;
	vol["Modifier"] = emitter.velocity_over_lifetime.modifier;

	shape["Enabled"] = emitter.shape.enabled;
	shape["Type"] = emitter.shape.shape;
	//#TODO: Work with optional stuff
	shape["Radius"] = *emitter.shape.radius;
	shape["Arc"] = *emitter.shape.arc;
	shape["Angle"] = *emitter.shape.angle;
	shape["RandomizeDirection"] = emitter.shape.randomize_direction;
	shape["RandomPositionOffset"] = emitter.shape.random_position_offset;
	shape["SpawnMode"] = *emitter.shape.mode;
	shape["Scale"] = emitter.shape.scale;
}			  

void to_json(nlohmann::json& json, Material const& material) {
	// clang-format off
    json["MaterialComponent"] = nlohmann::json::object();
	json["MaterialComponent"]["Shader"] = material.shader;
	json["MaterialComponent"]["Texture"] = material.texture;
    // clang-format on 
}

void to_json(nlohmann::json& json, Rotator const& rotator) {
	json["RotatorComponent"]  = nlohmann::json::object();
	json["RotatorComponent"]["Speed"] = rotator.speed;
}

} // namespace Saturn::Components
