#ifndef SATURN_PARTICLE_EMITTER_HPP_
#define SATURN_PARTICLE_EMITTER_HPP_

#include "AssetManager/AssetManager.hpp"
#include "AssetManager/Resource.hpp"
#include "ComponentBase.hpp"
#include "Math/Curve.hpp"
#include "Math/math_traits.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VertexArray.hpp"
#include "Utility/ColorGradient.hpp"

#include <glm/glm.hpp>
#include <optional>
#include <vector>

#include <nlohmann/json.hpp>

namespace Saturn {
namespace Systems {
class ParticleSystem;
}
} // namespace Saturn

namespace Saturn::Components {

struct [[component, category("Particle System")]] ParticleEmitter : ComponentBase {
    ParticleEmitter() {
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

        // Add additional rendering info
        VertexArray::CreateInfo vao_info;
        vao_info.attributes.push_back({0, 3}); // position
        vao_info.attributes.push_back({1, 2}); // texture coordinates
        vao_info.vertices = particle_quad_vertices;
        vao_info.indices = particle_quad_indices;
        particle_vao =
            AssetManager<VertexArray>::get_resource(vao_info, "particle_vao");
        VertexArray::BufferInfo pos_buffer_info;
        pos_buffer_info.attributes.push_back({2, 3, 1}); // position
        pos_buffer_info.mode = BufferMode::DataStream;

        pos_buffer_info.data = make_float_vec(particle_data.positions);
        VertexArray::BufferInfo scale_buffer_info;
        scale_buffer_info.attributes.push_back({3, 3, 1}); // scale
        scale_buffer_info.mode = BufferMode::DataStream;
        scale_buffer_info.data = make_float_vec(particle_data.sizes);
        VertexArray::BufferInfo color_buffer_info;
        color_buffer_info.attributes.push_back({4, 4, 1}); // color
        color_buffer_info.mode = BufferMode::DataStream;
        color_buffer_info.data = make_float_vec(particle_data.colors);

        particle_vao->add_buffer(pos_buffer_info);
        particle_vao->add_buffer(scale_buffer_info);
        particle_vao->add_buffer(color_buffer_info);
    }

    ParticleEmitter(ParticleEmitter const&) = default;
    ParticleEmitter(ParticleEmitter &&) = default;

    ParticleEmitter& operator=(ParticleEmitter const&) = default;
    ParticleEmitter& operator=(ParticleEmitter&&) = default;

    struct Particle {
        float life_left;
        float velocity;
        glm::vec3 direction;
    };

    struct MainModule {
        // If disabled, no particles will spawn
        bool enabled = true;

        float start_lifetime = 1.0f;

        float start_velocity = 1.0f;
        glm::vec4 start_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec2 start_size = glm::vec2(1.0f, 1.0f);

        // Total duration for which particles should spawn. In seconds
        float duration = 5.0f;
        bool loop = true;

        std::size_t max_particles = 1000;
    };

    struct EmissionModule {
        // If disabled, no particles will spawn
        bool enabled = true;
        float spawn_rate = 1.0f;
    };

    struct VelocityOverTimeModule {
        bool enabled = false;
        Math::Curve modifier = {Math::CurveShape::Constant, 1.0f, 1.0f};
    };

    struct SizeOverTimeModule {
        bool enabled = false;
        Math::Curve modifier = {Math::CurveShape::Constant, 1.0f, 1.0f};
    };

    struct ColorOverTimeModule {
        bool enabled = false;
        ColorGradient gradient;
    };

    enum class SpawnShape { Sphere = 0, Hemisphere, Cone, Box };
    enum class SpawnMode { Random };

    struct ShapeModule {
        bool enabled = true;

        SpawnShape shape = SpawnShape::Sphere;

        // Only present if shape is a Sphere, Hemisphere or a Cone
        std::optional<float> radius = 1.0f;
        // Only present if shape is a Cone. Represents the angle of the Cone, in
        // degrees
        std::optional<float> angle = 60.0f;
        // Only present if shape is a Cone, Sphere or Hemisphere. Represents the
        // area of the circle to use (in degrees)
        std::optional<float> arc = 360.0f;
        // Only present if shape is a Cone
        std::optional<SpawnMode> mode = SpawnMode::Random;

        // Must be in range [0, 1]
        float randomize_direction = 0.0f;
        //#MaybeTODO: Vector for x, y, z separate? (Optional)
        float random_position_offset = 0.0f;

        // Currently only used when shape is a Box
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    // Special effects
    bool additive = false;

    // Modules:
    MainModule main;
    EmissionModule emission;
    VelocityOverTimeModule velocity_over_lifetime;
    SizeOverTimeModule size_over_lifetime;
    ColorOverTimeModule color_over_lifetime;
    ShapeModule shape;

    std::vector<Particle> particles;
    Resource<VertexArray> particle_vao;
    Resource<Texture> texture;

    struct ParticleData {
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> sizes;
    };

    ParticleData particle_data;

    friend class Systems::ParticleSystem;
    friend class ::Saturn::Renderer;

private:
    float time_since_last_spawn = 0.0f;
    float time_since_start = 0.0f;
};

// Serializers for enums

// clang-format off
NLOHMANN_JSON_SERIALIZE_ENUM(ParticleEmitter::SpawnShape,
    {{ParticleEmitter::SpawnShape::Box, "Box"},
	{ParticleEmitter::SpawnShape::Cone, "Cone"},
    {ParticleEmitter::SpawnShape::Hemisphere, "Hemisphere"},
    {ParticleEmitter::SpawnShape::Sphere, "Sphere"},
    })
NLOHMANN_JSON_SERIALIZE_ENUM(ParticleEmitter::SpawnMode,
	{{ParticleEmitter::SpawnMode::Random, "Random"},
	})
// clang-format on

} // namespace Saturn::Components

#endif
