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
        camera.viewport_id = (*cam)["ViewportID"].get<std::size_t>();
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
        mesh.mesh = (*j)["Mesh"].get<Resource<Mesh>>();
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

        static const std::vector<float> particle_quad_vertices = {
            -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
            1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TR
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
            1.0f,  -1.0f, 0.0f, 1.0f, 0.0f  // BR
        };

        static const std::vector<GLuint> particle_quad_indices = {
            0, 1, 2, // First triangle
            1, 2, 3  // Second triangle
        };

        emitter.additive = (*emit)["Additive"].get<bool>();
        // Add additional rendering info
        VertexArray::CreateInfo vao_info;
        vao_info.attributes.push_back({0, 3}); // position
        vao_info.attributes.push_back({1, 2}); // texture coordinates
        vao_info.vertices = particle_quad_vertices;
        vao_info.indices = particle_quad_indices;
        emitter.particle_vao =
            AssetManager<VertexArray>::get_resource(vao_info, "particle_vao");
        VertexArray::BufferInfo pos_buffer_info;
        pos_buffer_info.attributes.push_back({2, 3, 1}); // position
        pos_buffer_info.mode = BufferMode::DataStream;
        //#TODO: Get rid of make_float_vec
        pos_buffer_info.data = make_float_vec(emitter.particle_data.positions);
        VertexArray::BufferInfo scale_buffer_info;
        scale_buffer_info.attributes.push_back({3, 3, 1}); // scale
        scale_buffer_info.mode = BufferMode::DataStream;
        scale_buffer_info.data = make_float_vec(emitter.particle_data.sizes);
        VertexArray::BufferInfo color_buffer_info;
        color_buffer_info.attributes.push_back({4, 4, 1}); // color
        color_buffer_info.mode = BufferMode::DataStream;
        color_buffer_info.data = make_float_vec(emitter.particle_data.colors);

        emitter.particle_vao->add_buffer(pos_buffer_info);
        emitter.particle_vao->add_buffer(scale_buffer_info);
        emitter.particle_vao->add_buffer(color_buffer_info);
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
        material.lit = (*j)["Lit"];
        if (material.lit) {
            material.diffuse_map = (*j)["DiffuseMap"].get<Resource<Texture>>();
            material.specular_map =
                (*j)["SpecularMap"].get<Resource<Texture>>();
            material.shininess = (*j)["Shininess"].get<float>();
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

void from_json(nlohmann::json const& json, PointLight& light) {
    auto p = json.find("PointLightComponent");
    if (p == json.end()) {
        throw std::runtime_error(
            "No PointLight component stored even though it was requested");
    } else {
        light.ambient = (*p)["Ambient"];
        light.diffuse = (*p)["Diffuse"];
        light.specular = (*p)["Specular"];
    }
}

void from_json(nlohmann::json const& json, DirectionalLight& light) {
    auto l = json.find("DirectionalLightComponent");
    if (l == json.end()) {
        throw std::runtime_error("No DirectionalLight component stored even "
                                 "though it was requested");
    } else {
        light.ambient = (*l)["Ambient"];
        light.diffuse = (*l)["Diffuse"];
        light.specular = (*l)["Specular"];
        light.direction = (*l)["Direction"];
    }
}

void from_json(nlohmann::json const& json, SpotLight& light) {
    //#TODO
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
		{"FOV", camera.fov},
		{"ViewportID", camera.viewport_id}
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
    json["StaticMeshComponent"] = nlohmann::json::object();
	json["StaticMeshComponent"]["Mesh"] = mesh.mesh;
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
	json["MaterialComponent"]["Lit"] = material.lit;
	json["MaterialComponent"]["DiffuseMap"] = material.diffuse_map;
	json["MaterialComponent"]["SpecularMap"] = material.specular_map;
	json["MaterialComponent"]["Shininess"] = material.shininess;
    // clang-format on 
}

void to_json(nlohmann::json& json, Rotator const& rotator) {
	json["RotatorComponent"]  = nlohmann::json::object();
	json["RotatorComponent"]["Speed"] = rotator.speed;
	json["RotatorComponent"]["EulerAngles"] = rotator.euler_angles;
}

void to_json(nlohmann::json& json, PointLight const& light) {
	json["PointLightComponent"] = nlohmann::json::object();
	json["PointLightComponent"]["Ambient"] = light.ambient;
	json["PointLightComponent"]["Diffuse"] = light.diffuse;
	json["PointLightComponent"]["Specular"] = light.specular;
}

void to_json(nlohmann::json& json, DirectionalLight const& light) {
	json["DirectionalLightComponent"] = nlohmann::json::object();
	json["PointLightComponent"]["Ambient"] = light.ambient;
	json["PointLightComponent"]["Diffuse"] = light.diffuse;
	json["PointLightComponent"]["Specular"] = light.specular;
	json["DirectionalLightComponent"]["Direction"] = light.direction;
}

void to_json(nlohmann::json& json, SpotLight const& light) {
	//#TODO
}

} // namespace Saturn::Components
