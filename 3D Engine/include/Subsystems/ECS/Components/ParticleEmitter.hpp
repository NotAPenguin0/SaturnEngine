#ifndef MVG_PARTICLE_EMITTER_HPP_
#define MVG_PARTICLE_EMITTER_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Math/Curve.hpp"
#include "Subsystems/Math/math_traits.hpp"
#include "Subsystems/Renderer/Texture.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"
#include "Utility/ColorGradient.hpp"

#include <GLM/glm.hpp>
#include <optional>
#include <vector>

namespace Saturn {
namespace Systems {
class ParticleSystem;
}
} // namespace Saturn

namespace Saturn::Components {

struct ParticleEmitter : ComponentBase {
    struct Particle {
        // #TODO: Better duration type
        float life_left;
        glm::vec4 color;
        glm::vec3 position;
        float velocity;
        glm::vec3 direction;
        glm::vec2 size;
    };

    struct MainModule {
        // If disabled, no particles will spawn
        bool enabled = true;

        // #TODO: Better duration type
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
        Math::Curve modifier = {Math::CurveShape::Constant, 1.0f};
    };

    struct SizeOverTimeModule {
        bool enabled = false;
        Math::Curve modifier = {Math::CurveShape::Constant, 1.0f};
    };

    struct ColorOverTimeModule {
        bool enabled = false;
        ColorGradient gradient;
    };

    enum class SpawnShape { Sphere, Hemisphere, Cone };
    enum class SpawnMode { Random };

    struct ShapeModule {
        bool enabled = true;

        SpawnShape shape;

        // Only present if shape is a Sphere or a Hemisphere
        std::optional<float> radius = 1.0f;
        // Only present if shape is a Cone
        std::optional<float> angle;
        // Only present if shape is a Cone. Represents the area of the circle to
        // use (in degrees)
        std::optional<float> arc;
		// Only present if shape is a Cone
        std::optional<SpawnMode> mode;


        // Must be in range [0, 1] //#Enforce
        float randomize_direction = 0.0f;
        //#MaybeTODO: Vector for x, y, z separate? (Optional)
        float random_position_offset = 0.0f;
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

    friend class Systems::ParticleSystem;

private:
    float time_since_last_spawn = 0.0f;
    float time_since_start = 0.0f;
};

} // namespace Saturn::Components

#endif
