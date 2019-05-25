#ifndef MVG_PARTICLE_SYSTEM_HPP_
#define MVG_PARTICLE_SYSTEM_HPP_

#include "../Components/ParticleEmitter.hpp"
#include "SystemBase.hpp"

namespace Saturn::Systems {

//#MaybeTODO: GPU particle system like UE4?
class ParticleSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;

private:
    std::size_t particles_to_spawn(float& time_since_last_spawn,
                                   float spawn_rate,
                                   float time_delta);

    void remove_expired_particles(Components::ParticleEmitter& emitter);
    void spawn_particle(Components::ParticleEmitter& emitter);
    void update_particle(Components::ParticleEmitter::Particle& particle,
                         Components::ParticleEmitter& emitter);

    float value_over_lifetime(Components::ParticleEmitter& emitter,
                              Components::ParticleEmitter::Particle& particle,
                              Math::Curve const& curve);

	glm::vec3 random_direction(glm::vec3 const& base, float randomness);
	glm::vec3 direction_in_sphere(float randomness);
    glm::vec3 direction_in_hemisphere(float randomness);
};

} // namespace Saturn::Systems

#endif
