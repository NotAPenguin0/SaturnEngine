#include "Serialization/ComponentMetaInfo.hpp"

namespace Saturn::Meta::detail {

// This function was generated by SaturnEngine
void init_component_meta_data(std::vector<ComponentInfo>& data) {
	{
		ComponentInfo info;
		info.name = "BoxCollider ";
		info.category = "Physics";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["center"].type = "glm::vec3";
		info.fields["center"].tooltip = "The center of the collider.";
		info.fields["center"].hide_in_editor = false;
		info.fields["half_widths"].type = "glm::vec3";
		info.fields["half_widths"].tooltip = "Half the width of the box for each axis.";
		info.fields["half_widths"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Button ";
		info.category = "UI";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["last_event"].type = "Saturn::Components::Button::Event";
		info.fields["last_event"].tooltip = "";
		info.fields["last_event"].hide_in_editor = true;
		info.fields["position"].type = "glm::vec2";
		info.fields["position"].tooltip = "Position on the UI canvas, relative to it's size.";
		info.fields["position"].hide_in_editor = false;
		info.fields["image"].type = "Resource<Saturn::Texture>";
		info.fields["image"].tooltip = "Background image to display.";
		info.fields["image"].hide_in_editor = false;
		info.fields["text_color"].type = "Saturn::color3";
		info.fields["text_color"].tooltip = "The text color.";
		info.fields["text_color"].hide_in_editor = false;
		info.fields["size"].type = "glm::vec2";
		info.fields["size"].tooltip = "Size of the button";
		info.fields["size"].hide_in_editor = false;
		info.fields["anchor"].type = "ui_anchors::anchor_t";
		info.fields["anchor"].tooltip = "UI anchor to align the button with.";
		info.fields["anchor"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Camera ";
		info.category = "Camera";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["up"].type = "glm::vec3";
		info.fields["up"].tooltip = "Up vector of the camera.";
		info.fields["up"].hide_in_editor = false;
		info.fields["front"].type = "glm::vec3";
		info.fields["front"].tooltip = "Front vector of the camera.";
		info.fields["front"].hide_in_editor = false;
		info.fields["fov"].type = "float";
		info.fields["fov"].tooltip = "Camera field of view (in degrees).";
		info.fields["fov"].hide_in_editor = false;
		info.fields["skybox"].type = "Resource<Saturn::CubeMap>";
		info.fields["skybox"].tooltip = "The skybox to render with this camera";
		info.fields["skybox"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "CameraZoomController ";
		info.category = "Camera";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["zoom_speed"].type = "float";
		info.fields["zoom_speed"].tooltip = "The speed of the zoom. Note that zoom corresponds to FOV.";
		info.fields["zoom_speed"].hide_in_editor = false;
		info.fields["min_zoom"].type = "float";
		info.fields["min_zoom"].tooltip = "Minimum zoom in degrees. Always greater than zero.";
		info.fields["min_zoom"].hide_in_editor = false;
		info.fields["max_zoom"].type = "float";
		info.fields["max_zoom"].tooltip = "Maximum zoom in degrees. Has to be smaller than 180";
		info.fields["max_zoom"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Canvas ";
		info.category = "UI";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["position"].type = "glm::vec2";
		info.fields["position"].tooltip = "The position of the canvas in 2D screen coordinates ranging from 0 to 1. Left top is (0, 0).";
		info.fields["position"].hide_in_editor = false;
		info.fields["size"].type = "glm::vec2";
		info.fields["size"].tooltip = "Size of the canvas relative to the screen size.";
		info.fields["size"].hide_in_editor = false;
		info.fields["opacity"].type = "float";
		info.fields["opacity"].tooltip = "Opacity of the UI overlay drawn on this canvas.";
		info.fields["opacity"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "ClickEffect ";
		info.category = "User-defined";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["sound_effect"].type = "Resource<audeo::SoundSource>";
		info.fields["sound_effect"].tooltip = "Sound effect to play on mouse click.";
		info.fields["sound_effect"].hide_in_editor = false;
		info.fields["max_distance"].type = "float";
		info.fields["max_distance"].tooltip = "The maximum distance the effect can be heard from.";
		info.fields["max_distance"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "ColliderRenderer ";
		info.category = "Editor-only";
		info.default_serialize = true;
		info.hide_in_editor = true;
		info.editor_only = false;
		
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "DirectionalLight ";
		info.category = "Light";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["ambient"].type = "Saturn::color3";
		info.fields["ambient"].tooltip = "Ambient light color.";
		info.fields["ambient"].hide_in_editor = false;
		info.fields["diffuse"].type = "Saturn::color3";
		info.fields["diffuse"].tooltip = "Diffuse light color.";
		info.fields["diffuse"].hide_in_editor = false;
		info.fields["specular"].type = "Saturn::color3";
		info.fields["specular"].tooltip = "Specular light color.";
		info.fields["specular"].hide_in_editor = false;
		info.fields["direction"].type = "glm::vec3";
		info.fields["direction"].tooltip = "Light direction vector.";
		info.fields["direction"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "DoNotSerialize ";
		info.category = "";
		info.default_serialize = false;
		info.hide_in_editor = true;
		info.editor_only = false;
		
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "EditorCameraController ";
		info.category = "Editor-only";
		info.default_serialize = true;
		info.hide_in_editor = true;
		info.editor_only = false;
		info.fields["speed"].type = "float";
		info.fields["speed"].tooltip = "";
		info.fields["speed"].hide_in_editor = false;
		info.fields["sensitivity"].type = "float";
		info.fields["sensitivity"].tooltip = "";
		info.fields["sensitivity"].hide_in_editor = false;
		info.fields["zoom_speed"].type = "float";
		info.fields["zoom_speed"].tooltip = "";
		info.fields["zoom_speed"].hide_in_editor = false;
		info.fields["invert_x"].type = "bool";
		info.fields["invert_x"].tooltip = "";
		info.fields["invert_x"].hide_in_editor = false;
		info.fields["invert_y"].type = "bool";
		info.fields["invert_y"].tooltip = "";
		info.fields["invert_y"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "FPSCameraController ";
		info.category = "Camera";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["speed"].type = "float";
		info.fields["speed"].tooltip = "The movement speed";
		info.fields["speed"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "FreeLookController ";
		info.category = "Camera";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["mouse_sensitivity"].type = "float";
		info.fields["mouse_sensitivity"].tooltip = "The sensitivity of the mouse controller";
		info.fields["mouse_sensitivity"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Image ";
		info.category = "UI";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["image"].type = "Resource<Saturn::Texture>";
		info.fields["image"].tooltip = "The image to display.";
		info.fields["image"].hide_in_editor = false;
		info.fields["position"].type = "glm::vec2";
		info.fields["position"].tooltip = "Position on the UI canvas, relative to it's size.";
		info.fields["position"].hide_in_editor = false;
		info.fields["anchor"].type = "ui_anchors::anchor_t";
		info.fields["anchor"].tooltip = "UI anchor to align the image with.";
		info.fields["anchor"].hide_in_editor = false;
		info.fields["size"].type = "glm::vec2";
		info.fields["size"].tooltip = "Size of the image.";
		info.fields["size"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "MusicController ";
		info.category = "Sound";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["music"].type = "Resource<audeo::SoundSource>";
		info.fields["music"].tooltip = "The music source to play.";
		info.fields["music"].hide_in_editor = false;
		info.fields["loop"].type = "bool";
		info.fields["loop"].tooltip = "Whether to loop the music when it has finished playing.";
		info.fields["loop"].hide_in_editor = false;
		info.fields["fade_in_ms"].type = "float";
		info.fields["fade_in_ms"].tooltip = "The amount of ms to fade in the music when starting playback.";
		info.fields["fade_in_ms"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Name ";
		info.category = "Editor-only";
		info.default_serialize = true;
		info.hide_in_editor = true;
		info.editor_only = false;
		info.fields["name"].type = "std::string";
		info.fields["name"].tooltip = "";
		info.fields["name"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "OutlineRenderer ";
		info.category = "Editor-only";
		info.default_serialize = true;
		info.hide_in_editor = true;
		info.editor_only = false;
		info.fields["color"].type = "Saturn::color3";
		info.fields["color"].tooltip = "";
		info.fields["color"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "ParticleEmitter ";
		info.category = "Particle System";
		info.default_serialize = false;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["start_size"].type = "glm::vec2";
		info.fields["start_size"].tooltip = "";
		info.fields["start_size"].hide_in_editor = false;
		info.fields["scale"].type = "glm::vec3";
		info.fields["scale"].tooltip = "";
		info.fields["scale"].hide_in_editor = false;
		info.fields["life_left"].type = "float";
		info.fields["life_left"].tooltip = "";
		info.fields["life_left"].hide_in_editor = false;
		info.fields["direction"].type = "glm::vec3";
		info.fields["direction"].tooltip = "";
		info.fields["direction"].hide_in_editor = false;
		info.fields["velocity"].type = "float";
		info.fields["velocity"].tooltip = "";
		info.fields["velocity"].hide_in_editor = false;
		info.fields["enabled"].type = "bool";
		info.fields["enabled"].tooltip = "";
		info.fields["enabled"].hide_in_editor = false;
		info.fields["start_velocity"].type = "float";
		info.fields["start_velocity"].tooltip = "";
		info.fields["start_velocity"].hide_in_editor = false;
		info.fields["start_lifetime"].type = "float";
		info.fields["start_lifetime"].tooltip = "";
		info.fields["start_lifetime"].hide_in_editor = false;
		info.fields["radius"].type = "std::optional<float>";
		info.fields["radius"].tooltip = "";
		info.fields["radius"].hide_in_editor = false;
		info.fields["start_color"].type = "glm::vec4";
		info.fields["start_color"].tooltip = "";
		info.fields["start_color"].hide_in_editor = false;
		info.fields["duration"].type = "float";
		info.fields["duration"].tooltip = "";
		info.fields["duration"].hide_in_editor = false;
		info.fields["loop"].type = "bool";
		info.fields["loop"].tooltip = "";
		info.fields["loop"].hide_in_editor = false;
		info.fields["max_particles"].type = "std::size_t";
		info.fields["max_particles"].tooltip = "";
		info.fields["max_particles"].hide_in_editor = false;
		info.fields["spawn_rate"].type = "float";
		info.fields["spawn_rate"].tooltip = "";
		info.fields["spawn_rate"].hide_in_editor = false;
		info.fields["modifier"].type = "Math::Curve";
		info.fields["modifier"].tooltip = "";
		info.fields["modifier"].hide_in_editor = false;
		info.fields["gradient"].type = "Saturn::ColorGradient";
		info.fields["gradient"].tooltip = "";
		info.fields["gradient"].hide_in_editor = false;
		info.fields["additive"].type = "bool";
		info.fields["additive"].tooltip = "";
		info.fields["additive"].hide_in_editor = false;
		info.fields["shape"].type = "Saturn::Components::ParticleEmitter::SpawnShape";
		info.fields["shape"].tooltip = "";
		info.fields["shape"].hide_in_editor = false;
		info.fields["angle"].type = "std::optional<float>";
		info.fields["angle"].tooltip = "";
		info.fields["angle"].hide_in_editor = false;
		info.fields["arc"].type = "std::optional<float>";
		info.fields["arc"].tooltip = "";
		info.fields["arc"].hide_in_editor = false;
		info.fields["positions"].type = "std::vector<glm::vec3>";
		info.fields["positions"].tooltip = "";
		info.fields["positions"].hide_in_editor = false;
		info.fields["mode"].type = "std::optional<SpawnMode>";
		info.fields["mode"].tooltip = "";
		info.fields["mode"].hide_in_editor = false;
		info.fields["randomize_direction"].type = "float";
		info.fields["randomize_direction"].tooltip = "";
		info.fields["randomize_direction"].hide_in_editor = false;
		info.fields["random_position_offset"].type = "float";
		info.fields["random_position_offset"].tooltip = "";
		info.fields["random_position_offset"].hide_in_editor = false;
		info.fields["emission"].type = "Saturn::Components::ParticleEmitter::EmissionModule";
		info.fields["emission"].tooltip = "";
		info.fields["emission"].hide_in_editor = false;
		info.fields["main"].type = "Saturn::Components::ParticleEmitter::MainModule";
		info.fields["main"].tooltip = "";
		info.fields["main"].hide_in_editor = false;
		info.fields["velocity_over_lifetime"].type = "Saturn::Components::ParticleEmitter::VelocityOverTimeModule";
		info.fields["velocity_over_lifetime"].tooltip = "";
		info.fields["velocity_over_lifetime"].hide_in_editor = false;
		info.fields["size_over_lifetime"].type = "Saturn::Components::ParticleEmitter::SizeOverTimeModule";
		info.fields["size_over_lifetime"].tooltip = "";
		info.fields["size_over_lifetime"].hide_in_editor = false;
		info.fields["color_over_lifetime"].type = "Saturn::Components::ParticleEmitter::ColorOverTimeModule";
		info.fields["color_over_lifetime"].tooltip = "";
		info.fields["color_over_lifetime"].hide_in_editor = false;
		info.fields["particles"].type = "std::vector<Particle>";
		info.fields["particles"].tooltip = "";
		info.fields["particles"].hide_in_editor = false;
		info.fields["particle_vao"].type = "Resource<Saturn::VertexArray>";
		info.fields["particle_vao"].tooltip = "";
		info.fields["particle_vao"].hide_in_editor = false;
		info.fields["texture"].type = "Resource<Saturn::Texture>";
		info.fields["texture"].tooltip = "";
		info.fields["texture"].hide_in_editor = false;
		info.fields["colors"].type = "std::vector<glm::vec4>";
		info.fields["colors"].tooltip = "";
		info.fields["colors"].hide_in_editor = false;
		info.fields["sizes"].type = "std::vector<glm::vec3>";
		info.fields["sizes"].tooltip = "";
		info.fields["sizes"].hide_in_editor = false;
		info.fields["particle_data"].type = "Saturn::Components::ParticleEmitter::ParticleData";
		info.fields["particle_data"].tooltip = "";
		info.fields["particle_data"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "PointLight ";
		info.category = "Light";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["ambient"].type = "Saturn::color3";
		info.fields["ambient"].tooltip = "Ambient light color.";
		info.fields["ambient"].hide_in_editor = false;
		info.fields["diffuse"].type = "Saturn::color3";
		info.fields["diffuse"].tooltip = "Diffuse light color.";
		info.fields["diffuse"].hide_in_editor = false;
		info.fields["specular"].type = "Saturn::color3";
		info.fields["specular"].tooltip = "Specular light color.";
		info.fields["specular"].hide_in_editor = false;
		info.fields["intensity"].type = "float";
		info.fields["intensity"].tooltip = "Light intensity.";
		info.fields["intensity"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Rigidbody ";
		info.category = "Physics";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["mass"].type = "float";
		info.fields["mass"].tooltip = "The mass of the rigid body.";
		info.fields["mass"].hide_in_editor = false;
		info.fields["locked_axes"].type = "glm::bvec3";
		info.fields["locked_axes"].tooltip = "The axes that should not be affected by the physics system";
		info.fields["locked_axes"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Rotator ";
		info.category = "User-defined";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["speed"].type = "float";
		info.fields["speed"].tooltip = "Speed at which to rotate";
		info.fields["speed"].hide_in_editor = false;
		info.fields["euler_angles"].type = "glm::vec3";
		info.fields["euler_angles"].tooltip = "The angles around which to rotate (in degrees).";
		info.fields["euler_angles"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Sound ";
		info.category = "Sound";
		info.default_serialize = false;
		info.hide_in_editor = false;
		info.editor_only = false;
		
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "SoundListener ";
		info.category = "Sound";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["forward"].type = "glm::vec3";
		info.fields["forward"].tooltip = "The forward vector of the listener";
		info.fields["forward"].hide_in_editor = false;
		info.fields["position"].type = "glm::vec3";
		info.fields["position"].tooltip = "Position of the sound listener, relatieve to the entity position";
		info.fields["position"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "SpotLight ";
		info.category = "Light";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["inner_angle"].type = "float";
		info.fields["inner_angle"].tooltip = "This is where the intensity will begin to fade out. (in degrees).";
		info.fields["inner_angle"].hide_in_editor = false;
		info.fields["ambient"].type = "Saturn::color3";
		info.fields["ambient"].tooltip = "Ambient light color.";
		info.fields["ambient"].hide_in_editor = false;
		info.fields["diffuse"].type = "Saturn::color3";
		info.fields["diffuse"].tooltip = "Diffuse light color.";
		info.fields["diffuse"].hide_in_editor = false;
		info.fields["specular"].type = "Saturn::color3";
		info.fields["specular"].tooltip = "Specular light color.";
		info.fields["specular"].hide_in_editor = false;
		info.fields["intensity"].type = "float";
		info.fields["intensity"].tooltip = "Light intensity.";
		info.fields["intensity"].hide_in_editor = false;
		info.fields["direction"].type = "glm::vec3";
		info.fields["direction"].tooltip = "Directin in which the light shines.";
		info.fields["direction"].hide_in_editor = false;
		info.fields["outer_angle"].type = "float";
		info.fields["outer_angle"].tooltip = "This is where the intensity will be completely faded out. (in degrees).";
		info.fields["outer_angle"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "StaticMesh ";
		info.category = "Rendering";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["mesh"].type = "Resource<Saturn::Mesh>";
		info.fields["mesh"].tooltip = "The mesh resource that will be rendered";
		info.fields["mesh"].hide_in_editor = false;
		info.fields["material"].type = "Resource<Saturn::Material>";
		info.fields["material"].tooltip = "The material to use when rendering the mesh";
		info.fields["material"].hide_in_editor = false;
		info.fields["face_cull"].type = "bool";
		info.fields["face_cull"].tooltip = "Whether to do face culling on the mesh. Usually you want this, except for planes.";
		info.fields["face_cull"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "StaticModel ";
		info.category = "Rendering";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["model"].type = "Resource<Saturn::Model>";
		info.fields["model"].tooltip = "The model resource that will be rendered";
		info.fields["model"].hide_in_editor = false;
		info.fields["face_cull"].type = "bool";
		info.fields["face_cull"].tooltip = "Whether to do face culling on the model. Usually you want this, except for planes.";
		info.fields["face_cull"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Text ";
		info.category = "UI";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["text"].type = "std::string";
		info.fields["text"].tooltip = "The text string to display.";
		info.fields["text"].hide_in_editor = false;
		info.fields["position"].type = "glm::vec2";
		info.fields["position"].tooltip = "Position on the UI canvas, relative to it's size.";
		info.fields["position"].hide_in_editor = false;
		info.fields["anchor"].type = "ui_anchors::anchor_t";
		info.fields["anchor"].tooltip = "UI anchor to align the text with.";
		info.fields["anchor"].hide_in_editor = false;
		info.fields["size"].type = "glm::vec2";
		info.fields["size"].tooltip = "Size of the text.";
		info.fields["size"].hide_in_editor = false;
		info.fields["font"].type = "Resource<Saturn::Font>";
		info.fields["font"].tooltip = "Font resource to render the text with.";
		info.fields["font"].hide_in_editor = false;
		info.fields["color"].type = "Saturn::color3";
		info.fields["color"].tooltip = "Text color.";
		info.fields["color"].hide_in_editor = false;
		data.push_back(info);
	}
	{
		ComponentInfo info;
		info.name = "Transform ";
		info.category = "Rendering";
		info.default_serialize = true;
		info.hide_in_editor = false;
		info.editor_only = false;
		info.fields["position"].type = "glm::vec3";
		info.fields["position"].tooltip = "Position of the entity in world space, or relative to its parent.";
		info.fields["position"].hide_in_editor = false;
		info.fields["rotation"].type = "glm::vec3";
		info.fields["rotation"].tooltip = "Rotation of the entity in euler angles (relative to parent).";
		info.fields["rotation"].hide_in_editor = false;
		info.fields["scale"].type = "glm::vec3";
		info.fields["scale"].tooltip = "Scale of the entity, independent of parent.";
		info.fields["scale"].hide_in_editor = false;
		data.push_back(info);
	}
}


} // namespace Saturn::Meta::detail