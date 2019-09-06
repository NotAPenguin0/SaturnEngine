#ifndef SATURN_PARTICLE_EMITTER_HPP_
#define SATURN_PARTICLE_EMITTER_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Math/Curve.hpp"
#include "Subsystems/Math/math_traits.hpp"
#include "Subsystems/Renderer/Texture.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"
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

struct COMPONENT HIDE_IN_EDITOR ParticleEmitter : ComponentBase {
    struct Particle {
        float life_left;
        float velocity;
        glm::vec3 direction;
    };

    struct MainModule {
        // If disabled, no particles will spawn
        bool enabled = true;

        float start_lifetime;

        float start_velocity;
        glm::vec4 start_color;
        glm::vec2 start_size;

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

    enum class SpawnShape { Sphere, Hemisphere, Cone, Box };
    enum class SpawnMode { Random };

    struct ShapeModule {
        bool enabled = true;

        SpawnShape shape;

        // Only present if shape is a Sphere, Hemisphere or a Cone
        std::optional<float> radius = 1.0f;
        // Only present if shape is a Cone. Represents the angle of the Cone, in
        // degrees
        std::optional<float> angle;
        // Only present if shape is a Cone. Represents the area of the circle to
        // use (in degrees)
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
