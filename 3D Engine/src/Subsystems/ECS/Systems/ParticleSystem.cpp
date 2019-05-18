#include "Subsystems/ECS/Systems/ParticleSystem.hpp"

#include "Subsystems/ECS/Components/ParticleEmitter.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

#include <algorithm>

namespace Saturn::Systems {

void ParticleSystem::on_update(Scene& scene) {
    using namespace Components;

    for (auto [emitter] : scene.get_ecs().select<ParticleEmitter>()) {
        std::size_t new_particles =
            static_cast<std::size_t>(emitter.spawn_rate / Time::deltaTime);
        if (emitter.particles.size() + new_particles > emitter.max_particles) {
            new_particles = emitter.max_particles - emitter.particles.size();
        }

        // #MaybeOptimize Insertion/deletion of particles (maybe switch to
        // std::list since no random access is needed?)

        // First step: spawn new particles
        for (std::size_t i = 0; i < new_particles; ++i) {
            spawn_particle(emitter);
        }

        // Second step: update particles
        for (auto& particle : emitter.particles) {
            update_particle(particle, emitter);
        }

        // Third step: delete 'dead' particles
        remove_expired_particles(emitter);
    }
}

void ParticleSystem::remove_expired_particles(
    Components::ParticleEmitter& emitter) {
    using namespace Components;

    auto& particles = emitter.particles;
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](ParticleEmitter::Particle const& p) {
                                       return p.life_left < 0.0f;
                                   }),
                    particles.end());
}

void ParticleSystem::spawn_particle(Components::ParticleEmitter& emitter) {
    using namespace Components;

    // #ParticleSystemTODO: Dependency on transform? Probably needed but not
    // great ...
    // #ParticleSystemTODO: Make this work with entity parenting (absolute
    // transforms)
    auto& transform = emitter.entity->get_component<Transform>();

    ParticleEmitter::Particle particle;
    particle.life_left = emitter.start_lifetime;
    particle.color = {1.0, 0.0, 1.0}; // #ParticleSystemTemp
    particle.position = transform.position;
    particle.velocity = emitter.start_velocity;

    // Put the particle in the emitter
    emitter.particles.push_back(std::move(particle));
}

void ParticleSystem::update_particle(
    Components::ParticleEmitter::Particle& particle,
    Components::ParticleEmitter& emitter) {
    using namespace Components;
    (void)emitter;
    particle.life_left -= Time::deltaTime;
    if (particle.life_left > 0.0f) {
        particle.position += particle.velocity * Time::deltaTime;
    }
}

} // namespace Saturn::Systems
