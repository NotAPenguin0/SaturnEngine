// The from_json function in this file was generated code.
// When modifying other code, make sure to edit the template as well.
// At some point, I will move this generated function out of this file
// to avoid this problem. #TODO

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
#include "Subsystems/Serialization/CustomSerializers.hpp"

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

	{{#DeserializeComponent}}
	// Deserialization for {{ComponentName}} component
	if (auto const& component = j.find("{{ComponentName}}Component"); component != j.end()) {
		auto& c = obj.get_scene()->get_ecs().get_with_id<{{ComponentName}}>(
			obj.add_component<{{ComponentName}}>());
		j.get_to(c);
		{{{AdditionalCode}}}
	}
	{{/DeserializeComponent}}
} 

} // namespace Saturn
