#ifndef MVG_PARTICLE_EMITTER_HPP_
#define MVG_PARTICLE_EMITTER_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Math/Curve.hpp"
#include "Subsystems/Renderer/Texture.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"

#include <GLM/glm.hpp>
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
        glm::vec3 velocity;
        glm::vec2 size;
    };

    /*Basic Information*/

    float spawn_rate;
    // #TODO: Better duration type
    float start_lifetime;

    glm::vec3 start_velocity;
    glm::vec4 start_color;
    glm::vec2 start_size;

    // Total duration for which particles should spawn. In seconds
    float duration = 5.0f;
    bool loop = true;

    // Curves
    Math::Curve size_over_lifetime =
        Math::Curve{Math::CurveShape::Constant, 1.0f};
    Math::Curve velocity_over_lifetime =
        Math::Curve{Math::CurveShape::Constant, 1.0f};

    // Special effects
    bool glow = false; //#TODO: Find better name

    std::size_t max_particles = 1000;

    /*Spawn shape module*/

    enum class SpawnShape { Box };

    struct Shape {
        SpawnShape shape;

    } shape;

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
