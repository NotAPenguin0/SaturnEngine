#include <editor/widgets/display_component_fields.hpp>
#include <editor/widgets/display_field.hpp>

// Generated source file. Do not modify manually.

namespace editor {

// Note that we append the component name to the field name. This is to avoid ImGui being annoying about fields with the same names

void display_component_fields(saturn::components::Blueprint& component) {
    display_field dispatcher;
    dispatcher("blueprint##Blueprint", &component.blueprint);
    dispatcher("model##Blueprint", &component.model);
}
void display_component_fields(saturn::components::BlueprintInstance& component) {
    display_field dispatcher;
    dispatcher("blueprint##BlueprintInstance", &component.blueprint);
}
void display_component_fields(saturn::components::Camera& component) {
    display_field dispatcher;
    dispatcher("front##Camera", &component.front);
    dispatcher("up##Camera", &component.up);
    dispatcher("fov##Camera", &component.fov);
}
void display_component_fields(saturn::components::MeshRenderer& component) {
    display_field dispatcher;
    dispatcher("material##MeshRenderer", &component.material);
}
void display_component_fields(saturn::components::StaticMesh& component) {
    display_field dispatcher;
    dispatcher("mesh##StaticMesh", &component.mesh);
}
void display_component_fields(saturn::components::PointLight& component) {
    display_field dispatcher;
    dispatcher("ambient##PointLight", &component.ambient);
    dispatcher("diffuse##PointLight", &component.diffuse);
    dispatcher("specular##PointLight", &component.specular);
    dispatcher("intensity##PointLight", &component.intensity);
}
void display_component_fields(saturn::components::EditorCamera& component) {
    display_field dispatcher;
    dispatcher("sensitivity##EditorCamera", &component.sensitivity);
}
void display_component_fields(saturn::components::Transform& component) {
    display_field dispatcher;
    dispatcher("position##Transform", &component.position);
    dispatcher("rotation##Transform", &component.rotation);
    dispatcher("scale##Transform", &component.scale);
}
void display_component_fields(saturn::components::Rotator& component) {
    display_field dispatcher;
    dispatcher("speed##Rotator", &component.speed);
    dispatcher("axes##Rotator", &component.axes);
}

}