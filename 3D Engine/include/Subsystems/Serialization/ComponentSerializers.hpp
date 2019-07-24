#ifndef SATURN_COMPONENT_SERIALIZERS_HPP_
#define SATURN_COMPONENT_SERIALIZERS_HPP_

/*	This file was generated using the serialization code generation program 
 *	for SaturnEngine. Do NOT modify this file manually.
 */

#include <nlohmann/json.hpp>
        
namespace Saturn::Components {
        
struct Camera;
struct CameraZoomController;
struct DirectionalLight;
struct FPSCameraController;
struct FreeLookController;
struct Material;
struct PointLight;
struct Rigidbody;
struct Rotator;
struct SpotLight;
struct StaticMesh;
struct Transform;

void from_json(nlohmann::json const& json, Camera& component);
void from_json(nlohmann::json const& json, CameraZoomController& component);
void from_json(nlohmann::json const& json, DirectionalLight& component);
void from_json(nlohmann::json const& json, FPSCameraController& component);
void from_json(nlohmann::json const& json, FreeLookController& component);
void from_json(nlohmann::json const& json, Material& component);
void from_json(nlohmann::json const& json, PointLight& component);
void from_json(nlohmann::json const& json, Rigidbody& component);
void from_json(nlohmann::json const& json, Rotator& component);
void from_json(nlohmann::json const& json, SpotLight& component);
void from_json(nlohmann::json const& json, StaticMesh& component);
void from_json(nlohmann::json const& json, Transform& component);

void to_json(nlohmann::json& json, Camera const& component);
void to_json(nlohmann::json& json, CameraZoomController const& component);
void to_json(nlohmann::json& json, DirectionalLight const& component);
void to_json(nlohmann::json& json, FPSCameraController const& component);
void to_json(nlohmann::json& json, FreeLookController const& component);
void to_json(nlohmann::json& json, Material const& component);
void to_json(nlohmann::json& json, PointLight const& component);
void to_json(nlohmann::json& json, Rigidbody const& component);
void to_json(nlohmann::json& json, Rotator const& component);
void to_json(nlohmann::json& json, SpotLight const& component);
void to_json(nlohmann::json& json, StaticMesh const& component);
void to_json(nlohmann::json& json, Transform const& component);

} // namespace Saturn::Components
        
#endif