#ifndef MVG_PARTICLE_EMITTER_HPP_
#define MVG_PARTICLE_EMITTER_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"

#include <GLM/glm.hpp>
#include <vector>

namespace Saturn::Components {

struct ParticleEmitter : ComponentBase {
    struct Particle {
        // #TODO: Better duration type
        float life_left;
        glm::vec3 color;
        glm::vec3 position;
        glm::vec3 velocity;
    };

    float spawn_rate;
    // #TODO: Better duration type
    float start_lifetime;

    glm::vec3 start_velocity;

	std::size_t max_particles = 1000;

    // #Optimize Have a max_particles to limit reallocations
    std::vector<Particle> particles;
	Resource<VertexArray> particle_vao;
};

} // namespace Saturn::Components

#endif
