#include "Serialization/CustomSerializers.hpp"

#include "AssetManager/AssetManager.hpp"
#include "ECS/Components/ParticleEmitter.hpp"

namespace Saturn::Components {

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
    
    shape["Radius"] = *emitter.shape.radius;
    shape["Arc"] = *emitter.shape.arc;
    shape["Angle"] = *emitter.shape.angle;
    shape["RandomizeDirection"] = emitter.shape.randomize_direction;
    shape["RandomPositionOffset"] = emitter.shape.random_position_offset;
    shape["SpawnMode"] = *emitter.shape.mode;
    shape["Scale"] = emitter.shape.scale;
}

} // namespace Saturn::Components
