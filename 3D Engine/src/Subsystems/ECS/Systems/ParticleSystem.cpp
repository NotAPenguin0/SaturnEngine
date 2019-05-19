#include "Subsystems/ECS/Systems/ParticleSystem.hpp"

#include "Subsystems/ECS/Components/ParticleEmitter.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

#include <algorithm>

namespace Saturn::Systems {

std::size_t ParticleSystem::particles_to_spawn(
    float& time_since_last_spawn,
    float spawn_interval /* = 1/particles per second */,
    float time_delta) {
    time_since_last_spawn += time_delta;
    float diff = time_since_last_spawn - spawn_interval;
    if (diff >= 0.0f) {
        time_since_last_spawn = 0.0f;
        return (std::size_t)(diff / spawn_interval + 1.0f);
    } else {
        return 0;
    }
}

void ParticleSystem::on_update(Scene& scene) {
    using namespace Components;

    for (auto [emitter] : scene.get_ecs().select<ParticleEmitter>()) {

        // Check if we need to continue spawning particles

        std::size_t new_particles =
            particles_to_spawn(emitter.time_since_last_spawn,
                               1.0f / emitter.spawn_rate, Time::deltaTime);

        emitter.time_since_start += Time::deltaTime;
        //#TODO: Move this to particles_to_spawn()? Especially second check
        if (!emitter.loop) {
            // Check if effect has stopped
            if (emitter.time_since_start >= emitter.duration) {
                new_particles = 0;
            }
        }

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
    particle.color = emitter.start_color;
    particle.position = transform.position;
    particle.velocity = emitter.start_velocity;
    particle.size = emitter.start_size;

    // Put the particle in the emitter
    emitter.particles.push_back(std::move(particle));
}

void ParticleSystem::update_particle(
    Components::ParticleEmitter::Particle& particle,
    Components::ParticleEmitter& emitter) {
    using namespace Components;

    // Update life left
    particle.life_left -= Time::deltaTime;
    // If particle is still alive, update it
    if (particle.life_left > 0.0f) {
        // Update position
        particle.position += particle.velocity * Time::deltaTime;
        // Update scale
        particle.size = emitter.start_size *
                        glm::vec2(value_over_lifetime(
                            emitter, particle, emitter.size_over_lifetime));
		// Update velocity
        particle.velocity = emitter.start_velocity *
                            glm::vec3(value_over_lifetime(
                                emitter, particle, emitter.velocity_over_lifetime));
    }
}

float ParticleSystem::value_over_lifetime(
    Components::ParticleEmitter& emitter,
    Components::ParticleEmitter::Particle& particle,
    Math::Curve const& curve) {

    float curve_val = curve.get(emitter.start_lifetime - particle.life_left,
                                emitter.start_lifetime);

    // Map from [0, start_lifetime] to [0, scale]
    float new_val = Math::map_range(curve_val, 0.0f, emitter.start_lifetime,
                                    0.0f, curve.scale);
    return new_val;
}

} // namespace Saturn::Systems
