#include "ECS/Systems/ParticleSystem.hpp"

#include "ECS/Components/ParticleEmitter.hpp"
#include "Math/Math.hpp"
#include "Math/math_traits.hpp"
#include "Scene/Scene.hpp"
#include "Time/Time.hpp"

#include "Utility/Utility.hpp"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

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
        if (!emitter.main.enabled) {
            new_particles = 0;
        } else if (!emitter.main.loop) {
            // Check if effect has stopped
            if (emitter.time_since_start >= emitter.main.duration) {
                new_particles = 0;
            }
        }

        if (emitter.particles.size() + new_particles >
                emitter.main.max_particles &&
            emitter.main.max_particles >=
                emitter.particles.size() + new_particles) {
            new_particles =
                emitter.main.max_particles - emitter.particles.size();
        }

        // First step: spawn new particles
        auto trans = make_absolute_transform(
            emitter.entity->get_component<Components::Transform>());

        for (std::size_t i = 0; i < new_particles; ++i) {
            spawn_particle(emitter, trans);
        }

        // Second step: update particles
        for (std::size_t i = 0; i < emitter.particles.size(); ++i) {
            update_particle(i, emitter);
        }

        // Third step: delete 'dead' particles
        remove_expired_particles(emitter);

        // Final step: Update buffer data

        if (!emitter.particles.empty()) {
            emitter.particle_vao->update_buffer_data(
                1, glm::value_ptr(emitter.particle_data.positions[0]),
                3 * emitter.particle_data.positions.size());
            // Sizes
            emitter.particle_vao->update_buffer_data(
                2, glm::value_ptr(emitter.particle_data.sizes[0]),
                3 * emitter.particle_data.sizes.size());
            // Colors
            emitter.particle_vao->update_buffer_data(
                3, glm::value_ptr(emitter.particle_data.colors[0]),
                4 * emitter.particle_data.colors.size());
        }
    }
}

//#CHECK: Make particle erase itself in update_particle() for better
// performance?
void ParticleSystem::remove_expired_particles(
    Components::ParticleEmitter& emitter) {
    using namespace Components;

    auto& particles = emitter.particles;
    /*
        particles.erase(std::remove_if(particles.begin(), particles.end(),
                                       [](ParticleEmitter::Particle const& p) {
                                           return p.life_left < 0.0f;
                                       }),
                        particles.end());*/
    for (std::size_t i = 0; i < emitter.particles.size(); ++i) {
        if (emitter.particles[i].life_left < 0.0f) {
            emitter.particle_data.colors.erase(
                emitter.particle_data.colors.begin() + (long long)i);
            emitter.particle_data.positions.erase(
                emitter.particle_data.positions.begin() + (long long)i);
            emitter.particle_data.sizes.erase(
                emitter.particle_data.sizes.begin() + (long long)i);
            emitter.particles.erase(emitter.particles.begin() + (long long)i);
            --i;
        }
    }
}

void ParticleSystem::spawn_particle(
    Components::ParticleEmitter& emitter,
    Components::Transform const& abs_transform) {
    using namespace Components;

    // #ParticleSystemTODO: Dependency on transform? Probably needed but not
    // great ...
    auto& transform = emitter.entity->get_component<Transform>();

    ParticleEmitter::Particle particle;
    particle.life_left = emitter.main.start_lifetime;
    emitter.particle_data.colors.push_back(emitter.main.start_color);
    emitter.particle_data.positions.push_back(transform.position);

    auto& color = emitter.particle_data.colors.back();
    auto& position = emitter.particle_data.positions.back();

    // Direction and Position
    // Switch on shape and call the appropriate function
    switch (emitter.shape.shape) {
        case Components::ParticleEmitter::SpawnShape::Sphere:
            particle.direction = Math::direction_in_sphere(
                emitter.shape.randomize_direction, abs_transform.rotation);
            position += Math::position_on_sphere(*emitter.shape.radius);
            break;
        case ParticleEmitter::SpawnShape::Hemisphere:
            particle.direction = Math::direction_in_hemisphere(
                emitter.shape.randomize_direction, abs_transform.rotation);
            position = Math::position_on_hemisphere(*emitter.shape.radius,
                                                    abs_transform.rotation);
            break;
        case Components::ParticleEmitter::SpawnShape::Cone:
            position = Math::position_on_circle(*emitter.shape.radius,
                                                *emitter.shape.arc,
                                                abs_transform.rotation);
            particle.direction = Math::direction_in_cone(
                *emitter.shape.arc, *emitter.shape.angle,
                abs_transform.rotation);
            break;
        case Components::ParticleEmitter::SpawnShape::Box:
            position = Math::position_in_box(emitter.shape.scale,
                                             abs_transform.rotation);
            particle.direction = Math::direction_in_sphere(
                emitter.shape.randomize_direction, abs_transform.rotation);
            break;
    }

    // Update position to no longer use relative position to origin
    position += abs_transform.position;

    particle.velocity = emitter.main.start_velocity;
    emitter.particle_data.sizes.emplace_back(emitter.main.start_size, 1.0f);

    // Put the particle in the emitter
    emitter.particles.push_back(std::move(particle));
}

void ParticleSystem::update_particle(std::size_t index,
                                     Components::ParticleEmitter& emitter) {
    using namespace Components;

    auto& particle = emitter.particles[index];

    // Update life left
    particle.life_left -= Time::deltaTime;
    // If particle is still alive, update it
    if (particle.life_left > 0.0f) {
        // Update position
        emitter.particle_data.positions[index] +=
            particle.direction * particle.velocity * Time::deltaTime;
        // Update scale
        if (emitter.size_over_lifetime.enabled) {
            auto val = value_over_lifetime(emitter, particle,
                                           emitter.size_over_lifetime.modifier);
            auto v2 = glm::vec2(val);
            auto as_vec3 = glm::vec3(emitter.main.start_size * v2, 0.0f);
            emitter.particle_data.sizes[index] = as_vec3;
        }
        // Update velocity
        if (emitter.velocity_over_lifetime.enabled) {
            auto val_ratio = value_over_lifetime(
                emitter, particle, emitter.velocity_over_lifetime.modifier);
            particle.velocity = emitter.main.start_velocity * val_ratio;
        }

        // Update color
        if (emitter.color_over_lifetime.enabled) {
            // Needs to be mapped here manually because we use interpolate()
            // instead of value_over_lifetime()
            auto life_pct = Math::map_range(
                emitter.main.start_lifetime - particle.life_left, 0.0f,
                emitter.main.start_lifetime, 0.0f, 1.0f);
            emitter.particle_data.colors[index] =
                emitter.color_over_lifetime.gradient.interpolate(life_pct);
        }
    }
}

float ParticleSystem::value_over_lifetime(
    Components::ParticleEmitter& emitter,
    Components::ParticleEmitter::Particle& particle,
    Math::Curve const& curve) {

    float life_pct =
        Math::map_range(emitter.main.start_lifetime - particle.life_left, 0.0f,
                        emitter.main.start_lifetime, 0.0f, 1.0f);
    float curve_val = curve.get(life_pct);

    return curve_val;
}

} // namespace Saturn::Systems
