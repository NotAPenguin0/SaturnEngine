#include "Subsystems/ECS/Systems/ParticleSystem.hpp"

#include "Subsystems/ECS/Components/ParticleEmitter.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Math/math_traits.hpp"
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

        std::size_t new_particles = particles_to_spawn(
            emitter.time_since_last_spawn, 1.0f / emitter.emission.spawn_rate,
            Time::deltaTime);

        if (!emitter.emission.enabled) { new_particles = 0; }
        emitter.time_since_start += Time::deltaTime;
        //#TODO: Move this to particles_to_spawn()?
        if (!emitter.main.enabled) {
            new_particles = 0;
        } else if (!emitter.main.loop) {
            // Check if effect has stopped
            if (emitter.time_since_start >= emitter.main.duration) {
                new_particles = 0;
            }
        }

        if (emitter.particles.size() + new_particles >
            emitter.main.max_particles) {
            new_particles =
                emitter.main.max_particles - emitter.particles.size();
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
    particle.life_left = emitter.main.start_lifetime;
    particle.color = emitter.main.start_color;
    particle.position = transform.position;

    float randomness = emitter.shape.randomize_direction;

    // Start direction in spherical coordinates
    glm::vec3 start_direction;
    if (emitter.shape.shape == ParticleEmitter::SpawnShape::Sphere) {
        // Note that the radius is 1.0 since we want
        // our direction normalized. We can do this because the shape's radius
        // has no effect on the generated direction.
        start_direction = {1.0f, 0.0f, 0.0f};
    } else if (emitter.shape.shape ==
               Components::ParticleEmitter::SpawnShape::Hemisphere) {
        start_direction = {1.0f, 0.0f, 0.0f};
        // Make sure randomness direction is always on the hemisphere
        // #TODO: Split up all this logic in a function for each shape
        randomness /= 4.0f;
    }

    // Get direction
    glm::vec3 direction = Math::spherical_to_cartesian(start_direction);

    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    float theta_max = pi * randomness;
    float x = std::cos(2 * pi * r1) *
              std::sqrt(1 - std::pow((1 - r2 * (1 - std::cos(theta_max))), 2));
    float z = std::sin(2 * pi * r1) *
              std::sqrt(1 - std::pow((1 - r2 * (1 - std::cos(theta_max))), 2));
    float y = 1 - r2 * (1 - std::cos(theta_max));

    glm::quat rotation = glm::rotation(direction, glm::vec3(x, y, z));

    particle.direction = Math::rotate_vector_by_quaternion(direction, rotation);
    particle.velocity = emitter.main.start_velocity;
    particle.size = emitter.main.start_size;

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
        particle.position +=
            particle.direction * particle.velocity * Time::deltaTime;
        // Update scale
        if (emitter.size_over_lifetime.enabled) {
            particle.size =
                emitter.main.start_size *
                glm::vec2(value_over_lifetime(
                    emitter, particle, emitter.size_over_lifetime.modifier));
        }
        // Update velocity
        if (emitter.velocity_over_lifetime.enabled) {
            particle.velocity =
                emitter.main.start_velocity *
                value_over_lifetime(emitter, particle,
                                    emitter.velocity_over_lifetime.modifier);
        }

        // Update color
        if (emitter.color_over_lifetime.enabled) {
            particle.color = emitter.color_over_lifetime.gradient.interpolate(
                Math::map_range(emitter.main.start_lifetime -
                                    particle.life_left,
                                0.0f, emitter.main.start_lifetime, 0.0f, 1.0f));
        }
    }
}

float ParticleSystem::value_over_lifetime(
    Components::ParticleEmitter& emitter,
    Components::ParticleEmitter::Particle& particle,
    Math::Curve const& curve) {

    float curve_val =
        curve.get(emitter.main.start_lifetime - particle.life_left,
                  emitter.main.start_lifetime);

    auto curve_range = curve.output_range(emitter.main.start_lifetime);

    // Map from [0, start_lifetime] to [0, scale]
    float new_val = Math::map_range(curve_val, curve_range.min, curve_range.max,
                                    0.0f, curve.scale);
    return new_val;
}

} // namespace Saturn::Systems
