#ifndef MVG_COMPONENT_SERIALIZERS_HPP_
#define MVG_COMPONENT_SERIALIZERS_HPP_

#include <nlohmann/json.hpp>

namespace Saturn::Components {

struct Transform;
struct Camera;
struct FPSCameraController;
struct FreeLookController;
struct CameraZoomController;
struct StaticMesh;
struct Material;
struct ParticleEmitter;
struct Rotator;
struct PointLight;

void from_json(nlohmann::json const& json, Transform& transform);
void from_json(nlohmann::json const& json, Camera& camera);
void from_json(nlohmann::json const& json, FPSCameraController& fps);
void from_json(nlohmann::json const& json, FreeLookController& freelook);
void from_json(nlohmann::json const& json, CameraZoomController& zoom);
void from_json(nlohmann::json const& json, StaticMesh& mesh);
void from_json(nlohmann::json const& json, ParticleEmitter& emitter);
void from_json(nlohmann::json const& json, Material& material);
void from_json(nlohmann::json const& json, Rotator& rotator);
void from_json(nlohmann::json const& json, PointLight& light);

void to_json(nlohmann::json& json, Transform const& transform);
void to_json(nlohmann::json& json, Camera const& camera);
void to_json(nlohmann::json& json, FPSCameraController const& fps);
void to_json(nlohmann::json& json, FreeLookController const& freelook);
void to_json(nlohmann::json& json, CameraZoomController const& zoom);
void to_json(nlohmann::json& json, StaticMesh const& mesh);
void to_json(nlohmann::json& json, ParticleEmitter const& emitter);
void to_json(nlohmann::json& json, Material const& material);
void to_json(nlohmann::json& json, Rotator const& rotator);
void to_json(nlohmann::json& json, PointLight const& light);


} // namespace Saturn::Components

#endif
