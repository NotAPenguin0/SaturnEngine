#include "Subsystems/Scene/SceneObject.hpp"
#include "Subsystems/Scene/Scene.hpp"

#include "Core/Application.hpp"
#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/ECS/ComponentList.hpp"
#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Renderer/Renderer.hpp"
#include "Subsystems/Renderer/Viewport.hpp"
#include "Subsystems/Serialization/ComponentSerializers.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

namespace Saturn {

SceneObject::SceneObject(Scene* s, SceneObject* parent /*= nullptr*/) :
    scene(s), parent_obj(parent) {}

bool SceneObject::has_parent() const { return parent_obj != nullptr; }

SceneObject* SceneObject::parent() { return parent_obj; }

SceneObject const* SceneObject::parent() const { return parent_obj; }

void SceneObject::serialize_to_file(std::string_view path) {
    nlohmann::json json;
    json = *this;
    std::ofstream file(path.data());
    file << json.dump(4);
}

void to_json(nlohmann::json& j, SceneObject const& obj) {
    obj.serialize_components<COMPONENT_LIST>(j);
}

void from_json(nlohmann::json const& j, SceneObject& obj) {
    using namespace Components;
    auto& ecs = obj.get_scene()->get_ecs();

    // Transform component
    if (auto const& trans = j.find("TransformComponent"); trans != j.end()) {
        auto& transform = obj.get_scene()->get_ecs().get_with_id<Transform>(
            obj.add_component<Transform>());
        j.get_to(transform);
    }
    if (auto const& cam = j.find("CameraComponent"); cam != j.end()) {
        auto& camera = obj.get_scene()->get_ecs().get_with_id<Camera>(
            obj.add_component<Camera>());
        j.get_to(camera);
        obj.get_scene()
            ->get_app()
            ->get_renderer()
            ->get_viewport(camera.viewport_id)
            .set_camera(camera.id);
    }
    if (auto const& fps = j.find("FPSCameraControllerComponent");
        fps != j.end()) {
        auto& controller =
            obj.get_scene()->get_ecs().get_with_id<FPSCameraController>(
                obj.add_component<FPSCameraController>());
        j.get_to(controller);
    }
    if (auto const& freelook = j.find("FreeLookControllerComponent");
        freelook != j.end()) {
        auto& controller =
            obj.get_scene()->get_ecs().get_with_id<FreeLookController>(
                obj.add_component<FreeLookController>());
        j.get_to(controller);
    }
    if (auto const& zoom = j.find("CameraZoomControllerComponent");
        zoom != j.end()) {
        auto& controller =
            obj.get_scene()->get_ecs().get_with_id<CameraZoomController>(
                obj.add_component<CameraZoomController>());
        j.get_to(controller);
    }
    if (auto const& comp = j.find("StaticMeshComponent"); comp != j.end()) {
        auto& mesh = obj.get_scene()->get_ecs().get_with_id<StaticMesh>(
            obj.add_component<StaticMesh>());
        j.get_to(mesh);
    }
    if (auto const& comp = j.find("MaterialComponent"); comp != j.end()) {
        auto& mat = obj.get_scene()->get_ecs().get_with_id<Material>(
            obj.add_component<Material>());
        j.get_to(mat);
    }
    // ParticleEmitter component
    if (auto const& emit = j.find("ParticleEmitterComponent");
        emit != j.end()) {
        auto& emitter = obj.get_scene()->get_ecs().get_with_id<ParticleEmitter>(
            obj.add_component<ParticleEmitter>());
        j.get_to(emitter);
    }
    // Rotator component
    if (auto const& rot = j.find("RotatorComponent"); rot != j.end()) {
        auto& rotator = obj.get_scene()->get_ecs().get_with_id<Rotator>(
            obj.add_component<Rotator>());
        j.get_to(rotator);
    }
	if (auto const& pl = j.find("PointLightComponent"); pl != j.end()) {
        auto& light = obj.get_scene()->get_ecs().get_with_id<PointLight>(
            obj.add_component<PointLight>());
		j.get_to(light);
	}
} // namespace Saturn

} // namespace Saturn
