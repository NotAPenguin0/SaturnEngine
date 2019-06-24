#include "Subsystems/ECS/Systems/ParticleSystem.hpp"

#include "Subsystems/ECS/Components/ParticleEmitter.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Math/math_traits.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

#include "Utility/Utility.hpp"

#include <algorithm>
#include <GLM/gtc/type_ptr.hpp>

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

//#CHECK: Make particle erase itself in update_particle() for better performance?
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

    // #TODO: make this a switch
    if (emitter.shape.shape == ParticleEmitter::SpawnShape::Sphere) {
        particle.direction = direction_in_sphere(
            emitter.shape.randomize_direction, abs_transform.rotation);
        position += position_on_sphere(*emitter.shape.radius);
    } else if (emitter.shape.shape == ParticleEmitter::SpawnShape::Hemisphere) {
        particle.direction = direction_in_hemisphere(
            emitter.shape.randomize_direction, abs_transform.rotation);
        position = position_on_hemisphere(*emitter.shape.radius,
                                          abs_transform.rotation);
    } else if (emitter.shape.shape ==
               Components::ParticleEmitter::SpawnShape::Cone) {
        position = position_on_circle(*emitter.shape.radius, *emitter.shape.arc,
                                      abs_transform.rotation);
        particle.direction = direction_in_cone(
            *emitter.shape.arc, *emitter.shape.angle, abs_transform.rotation);
    } else if (emitter.shape.shape ==
               Components::ParticleEmitter::SpawnShape::Box) {
        position = position_in_box(emitter.shape.scale, abs_transform.rotation);
        particle.direction = direction_in_sphere(
            emitter.shape.randomize_direction, abs_transform.rotation);
    }

    //     // Randomize position
    //     particle.position = random_position(particle.position,
    //                                         emitter.shape.random_position_offset);

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
            emitter.particle_data.sizes[index] =
                glm::vec3(emitter.main.start_size *
                              glm::vec2(value_over_lifetime(
                                  emitter, particle,
                                  emitter.size_over_lifetime.modifier)),
                          0.0f);
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
            emitter.particle_data.colors[index] =
                emitter.color_over_lifetime.gradient.interpolate(
                    Math::map_range(
                        emitter.main.start_lifetime - particle.life_left, 0.0f,
                        emitter.main.start_lifetime, 0.0f, 1.0f));
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

glm::vec3 ParticleSystem::random_direction(glm::vec3 const& base,
                                           float randomness) {
    glm::vec3 direction = Math::spherical_to_cartesian(base);

    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    float theta_max = pi * randomness;
    float x = std::cos(2 * pi * r1) *
              std::sqrt(1 - std::pow((1 - r2 * (1 - std::cos(theta_max))), 2));
    float z = std::sin(2 * pi * r1) *
              std::sqrt(1 - std::pow((1 - r2 * (1 - std::cos(theta_max))), 2));
    float y = 1 - r2 * (1 - std::cos(theta_max));

    glm::quat rotation =
        glm::rotation(glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3(x, y, z));

    return Math::rotate_vector_by_quaternion(direction, rotation);
}

glm::vec3 ParticleSystem::direction_in_sphere(float randomness,
                                              glm::vec3 const& base_rotation) {
    const glm::vec3 base = Math::euler_to_spherical(base_rotation);
    return random_direction(base, randomness);
}

glm::vec3 ParticleSystem::direction_in_hemisphere(float randomness,
                                                  glm::vec3 base) {
    // Divide by 4 to transform given randomness for a full sphere to the
    // hemisphere
    return random_direction(Math::euler_to_spherical(base), randomness / 4.0f);
}

glm::vec3 ParticleSystem::random_position(glm::vec3 const& base,
                                          float max_offset) {
    float x = Math::RandomEngine::get(0.0f, max_offset);
    float y = Math::RandomEngine::get(0.0f, max_offset);
    float z = Math::RandomEngine::get(0.0f, max_offset);
    return base + glm::vec3{x, y, z};
}

glm::vec3 ParticleSystem::position_on_sphere(float radius) {
    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;
    float x = 2 * radius * std::cos(2 * pi * r1) * std::sqrt(r2 * (1 - r2));
    float y = 2 * radius * std::sin(2 * pi * r1) * std::sqrt(r2 * (1 - r2));
    float z = radius * (1 - 2 * r2);

    return {x, y, z};
}

glm::vec3 ParticleSystem::position_on_hemisphere(float radius,
                                                 glm::vec3 const& rotation) {
    // Trick the system by generating a direction on a unit hemisphere and then
    // projecting it onto our hemisphere
    glm::vec3 dir = direction_in_hemisphere(2.0f, rotation);

    return dir * radius;
}

glm::vec3 ParticleSystem::position_on_circle(float radius,
                                             float arc,
                                             glm::vec3 const& rotation) {
    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    float r2 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    float a = r1 * glm::radians(arc);
    float r = radius * std::sqrt(r2);

    float x = r * std::cos(a);
    float y = 0.0f;
    float z = r * std::sin(a);

    return Math::rotate_vector_by_quaternion(glm::vec3(x, y, z),
                                             glm::quat(rotation));
}

glm::vec3 ParticleSystem::direction_in_cone(float arc,
                                            float angle,
                                            glm::vec3 const& rotation) {
    float r1 = Math::RandomEngine::get(0.0f, 1.0f);
    static constexpr float pi = Math::math_traits<float>::pi;

    // fixed theta value because we're on a cone
    float theta = glm::radians(angle);
    // Random phi value on our defined arc
    float phi = r1 * glm::radians(arc);
    return Math::rotate_vector_by_quaternion(
        Math::spherical_to_cartesian(1.0f, theta, phi), glm::quat(rotation));
}

glm::vec3 ParticleSystem::position_in_box(glm::vec3 const& scale,
                                          glm::vec3 const& rotation) {
    glm::vec3 min = -(scale / 2.0f);
    glm::vec3 max = -min;
    float x = Math::RandomEngine::get(min.x, max.x);
    float y = Math::RandomEngine::get(min.y, max.y);
    float z = Math::RandomEngine::get(min.z, max.z);
    return Math::rotate_vector_by_quaternion(glm::vec3(x, y, z),
                                             glm::quat(rotation));
}

} // namespace Saturn::Systems
