#ifndef MVG_COMPONENT_SERIALIZERS_HPP_
#define MVG_COMPONENT_SERIALIZERS_HPP_

/*	This file was generated using the serialization code generation program 
 *	for SaturnEngine. Do NOT modify this file manually.
 */

#include <nlohmann/json.hpp>
        
namespace Saturn::Components {
        
struct BoxCollider;
struct Camera;
struct CameraZoomController;
struct ClickEffect;
struct ColliderRenderer;
struct DirectionalLight;
struct EditorCameraController;
struct FPSCameraController;
struct FreeLookController;
struct Material;
struct MusicController;
struct Name;
struct OutlineRenderer;
struct PointLight;
struct Rigidbody;
struct Rotator;
struct SoundListener;
struct SpotLight;
struct StaticMesh;
struct Transform;

void from_json(nlohmann::json const& json, BoxCollider& component);
void from_json(nlohmann::json const& json, Camera& component);
void from_json(nlohmann::json const& json, CameraZoomController& component);
void from_json(nlohmann::json const& json, ClickEffect& component);
void from_json(nlohmann::json const& json, ColliderRenderer& component);
void from_json(nlohmann::json const& json, DirectionalLight& component);
void from_json(nlohmann::json const& json, EditorCameraController& component);
void from_json(nlohmann::json const& json, FPSCameraController& component);
void from_json(nlohmann::json const& json, FreeLookController& component);
void from_json(nlohmann::json const& json, Material& component);
void from_json(nlohmann::json const& json, MusicController& component);
void from_json(nlohmann::json const& json, Name& component);
void from_json(nlohmann::json const& json, OutlineRenderer& component);
void from_json(nlohmann::json const& json, PointLight& component);
void from_json(nlohmann::json const& json, Rigidbody& component);
void from_json(nlohmann::json const& json, Rotator& component);
void from_json(nlohmann::json const& json, SoundListener& component);
void from_json(nlohmann::json const& json, SpotLight& component);
void from_json(nlohmann::json const& json, StaticMesh& component);
void from_json(nlohmann::json const& json, Transform& component);

void to_json(nlohmann::json& json, BoxCollider const& component);
void to_json(nlohmann::json& json, Camera const& component);
void to_json(nlohmann::json& json, CameraZoomController const& component);
void to_json(nlohmann::json& json, ClickEffect const& component);
void to_json(nlohmann::json& json, ColliderRenderer const& component);
void to_json(nlohmann::json& json, DirectionalLight const& component);
void to_json(nlohmann::json& json, EditorCameraController const& component);
void to_json(nlohmann::json& json, FPSCameraController const& component);
void to_json(nlohmann::json& json, FreeLookController const& component);
void to_json(nlohmann::json& json, Material const& component);
void to_json(nlohmann::json& json, MusicController const& component);
void to_json(nlohmann::json& json, Name const& component);
void to_json(nlohmann::json& json, OutlineRenderer const& component);
void to_json(nlohmann::json& json, PointLight const& component);
void to_json(nlohmann::json& json, Rigidbody const& component);
void to_json(nlohmann::json& json, Rotator const& component);
void to_json(nlohmann::json& json, SoundListener const& component);
void to_json(nlohmann::json& json, SpotLight const& component);
void to_json(nlohmann::json& json, StaticMesh const& component);
void to_json(nlohmann::json& json, Transform const& component);

} // namespace Saturn::Components
        
#endif