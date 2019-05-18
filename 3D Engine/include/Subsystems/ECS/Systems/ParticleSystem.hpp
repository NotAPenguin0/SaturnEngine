#ifndef MVG_PARTICLE_SYSTEM_HPP_
#define MVG_PARTICLE_SYSTEM_HPP_

#include "../Components/ParticleEmitter.hpp"
#include "SystemBase.hpp"

namespace Saturn::Systems {

class ParticleSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;

private:
    void remove_expired_particles(Components::ParticleEmitter& emitter);
    void spawn_particle(Components::ParticleEmitter& emitter);
    void update_particle(Components::ParticleEmitter::Particle& particle,
                         Components::ParticleEmitter& emitter);
};

} // namespace Saturn::Systems

#endif
