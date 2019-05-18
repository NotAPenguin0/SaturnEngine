#ifndef MVG_PARTICLE_EMITTER_HPP_
#define MVG_PARTICLE_EMITTER_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Texture.hpp"
#include "Subsystems/Renderer/VertexArray.hpp".

#include <GLM/glm.hpp>
#include <vector>

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

    float spawn_rate;
    // #TODO: Better duration type
    float start_lifetime;

    glm::vec3 start_velocity;
    glm::vec4 start_color;
    glm::vec2 start_size;

    std::size_t max_particles = 1000;

    std::vector<Particle> particles;
    Resource<VertexArray> particle_vao;
	Resource<Texture> texture;

    float time_since_last_spawn = 0.0f;
};

} // namespace Saturn::Components

#endif
