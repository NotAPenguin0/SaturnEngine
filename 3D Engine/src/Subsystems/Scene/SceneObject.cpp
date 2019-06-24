#include "Subsystems/Scene/SceneObject.hpp"
#include "Subsystems/Scene/Scene.hpp"

#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/Math/Math.hpp"

#include <nlohmann/json.hpp>
#include <vector>

namespace Saturn {

SceneObject::SceneObject(Scene* s, SceneObject* parent /*= nullptr*/) :
    scene(s), parent_obj(parent) {}

bool SceneObject::has_parent() const { return parent_obj != nullptr; }

SceneObject* SceneObject::parent() { return parent_obj; }

SceneObject const* SceneObject::parent() const { return parent_obj; }

void to_json(nlohmann::json& j, SceneObject const& obj) {
    //#TODO
    (void)j;
    (void)obj;
}

void from_json(nlohmann::json const& j, SceneObject& obj) {
    //#TODO: Refactor so component (de)serialization is auto generated maybe? Or
    // move to component classes

    using namespace Components;
    auto& ecs = obj.get_scene()->get_ecs();

    // Transform component
    if (auto const& trans = j.find("TransformComponent"); trans != j.end()) {
        // Transform component must be added
        auto& transform = obj.get_scene()->get_ecs().get_with_id<Transform>(
            obj.add_component<Transform>());
        transform.position = (*trans)["Position"].get<glm::vec3>();
        transform.rotation = (*trans)["Rotation"].get<glm::vec3>();
        transform.scale = (*trans)["Scale"].get<glm::vec3>();
    }
    // Rotator component
    if (auto const& rot = j.find("RotatorComponent"); rot != j.end()) {
        auto& rotator = obj.get_scene()->get_ecs().get_with_id<Rotator>(
            obj.add_component<Rotator>());
        rotator.euler_angles = (*rot)["EulerAngles"].get<glm::vec3>();
        rotator.speed = (*rot)["Speed"].get<float>();
    }
    // ParticleEmitter component
    if (auto const& emit = j.find("ParticleEmitterComponent");
        emit != j.end()) {
        auto& emitter = obj.get_scene()->get_ecs().get_with_id<ParticleEmitter>(
            obj.add_component<ParticleEmitter>());

        // Main module
        auto& main = (*emit)["Main"];
        emitter.main.enabled = main["Enabled"].get<bool>();
        emitter.main.start_lifetime = main["StartLifetime"].get<float>();
        emitter.main.start_velocity = main["StartVelocity"].get<float>();
        emitter.main.start_color = main["StartColor"].get<glm::vec4>();
        emitter.main.start_size = main["StartSize"].get<glm::vec2>();
        emitter.main.max_particles = main["MaxParticles"].get<std::size_t>();
        emitter.particles.reserve(emitter.main.max_particles);
        emitter.particle_data.colors.reserve(emitter.main.max_particles);
        emitter.particle_data.positions.reserve(emitter.main.max_particles);
        emitter.particle_data.sizes.reserve(emitter.main.max_particles);
        emitter.main.loop = main["Loop"].get<bool>();
        emitter.main.duration = main["Duration"].get<float>();

        // Emission module
        auto& emission = (*emit)["Emission"];
        emitter.emission.enabled = emission["Enabled"].get<bool>();
        emitter.emission.spawn_rate = emission["SpawnRate"].get<float>();

        // Color over lifetime module
        auto& col = (*emit)["ColorOverLifetime"];
        emitter.color_over_lifetime.enabled = col["Enabled"].get<bool>();
        if (emitter.color_over_lifetime.enabled) {
            emitter.color_over_lifetime.gradient.start =
                col["Gradient"]["Start"].get<glm::vec4>();
            emitter.color_over_lifetime.gradient.end =
                col["Gradient"]["End"].get<glm::vec4>();
        }

        // Size over lifetime module
        auto& sol = (*emit)["SizeOverLifetime"];
        emitter.size_over_lifetime.enabled = sol["Enabled"].get<bool>();
        if (emitter.size_over_lifetime.enabled) {
            emitter.size_over_lifetime.modifier =
                sol["Modifier"].get<Math::Curve>();
        }

        // Velocity over lifetime module
        auto& vol = (*emit)["VelocityOverLifetime"];
        emitter.velocity_over_lifetime.enabled = vol["Enabled"].get<bool>();
        if (emitter.velocity_over_lifetime.enabled) {
            emitter.velocity_over_lifetime.modifier =
                sol["Modifier"].get<Math::Curve>();
        }

        // Shape module
        auto& shape = (*emit)["Shape"];
        emitter.shape.enabled = shape["Enabled"].get<bool>();
        if (emitter.shape.enabled) {
            emitter.shape.shape =
                shape["Type"].get<ParticleEmitter::SpawnShape>();
            switch (emitter.shape.shape) {
                case ParticleEmitter::SpawnShape::Sphere:
                    emitter.shape.radius = shape["Radius"].get<float>();
                    emitter.shape.arc = shape["Arc"].get<float>();
                    emitter.shape.angle = shape["Angle"].get<float>();
                    emitter.shape.randomize_direction =
                        shape["RandomizeDirection"].get<float>();
                    emitter.shape.random_position_offset =
                        shape["RandomPositionOffset"].get<float>();
                    emitter.shape.mode =
                        shape["SpawnMode"].get<ParticleEmitter::SpawnMode>();
                    emitter.shape.scale = shape["Scale"].get<glm::vec3>();
                    break;
                case ParticleEmitter::SpawnShape::Hemisphere: break;
                case ParticleEmitter::SpawnShape::Cone: break;
                case ParticleEmitter::SpawnShape::Box: break;
            }
        }

        emitter.additive = (*emit)["additive"].get<bool>();
    }
}

} // namespace Saturn
