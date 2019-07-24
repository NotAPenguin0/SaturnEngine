#ifndef SATURN_PARTICLE_SYSTEM_HPP_
#define SATURN_PARTICLE_SYSTEM_HPP_

#include "../Components/ParticleEmitter.hpp"
#include "SystemBase.hpp"

namespace Saturn::Systems {

//#TODO: Particles on GPU using compute shaders
class ParticleSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;

private:
    std::size_t particles_to_spawn(float& time_since_last_spawn,
                                   float spawn_rate,
                                   float time_delta);

    void remove_expired_particles(Components::ParticleEmitter& emitter);
    void spawn_particle(Components::ParticleEmitter& emitter,
                        Components::Transform const& abs_transform);
    void update_particle(std::size_t index,
                         Components::ParticleEmitter& emitter);

    float value_over_lifetime(Components::ParticleEmitter& emitter,
                              Components::ParticleEmitter::Particle& particle,
                              Math::Curve const& curve);
};

} // namespace Saturn::Systems

#endif
