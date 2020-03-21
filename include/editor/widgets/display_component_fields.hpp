#ifndef SATURN_EDITOR_DISPLAY_COMPONENT_FIELDS_GENERATED_HPP_
#define SATURN_EDITOR_DISPLAY_COMPONENT_FIELDS_GENERATED_HPP_

#include <saturn/components/blueprint.hpp>
#include <saturn/components/blueprint_instance.hpp>
#include <saturn/components/camera.hpp>
#include <saturn/components/mesh_renderer.hpp>
#include <saturn/components/name.hpp>
#include <saturn/components/point_light.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/transform.hpp>
#include <samples/components/rotator.hpp>
#include <editor/components/editor_camera.hpp>

namespace editor {

void display_component_fields(saturn::components::Blueprint& component);
void display_component_fields(saturn::components::BlueprintInstance& component);
void display_component_fields(saturn::components::Camera& component);
void display_component_fields(saturn::components::MeshRenderer& component);
void display_component_fields(saturn::components::Name& component);
void display_component_fields(saturn::components::PointLight& component);
void display_component_fields(saturn::components::StaticMesh& component);
void display_component_fields(saturn::components::Transform& component);
void display_component_fields(saturn::components::Rotator& component);
void display_component_fields(saturn::components::EditorCamera& component);

}

#endif