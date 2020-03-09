/*	This file was generated using the serialization code generation program 
 *	for SaturnEngine. Do NOT modify this file manually.
 */
 
#include "Serialization/ComponentSerializers.hpp"
#include "Serialization/CustomSerializers.hpp"
#include "AssetManager/AssetManager.hpp"
#include "ECS/Components.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace Saturn::Components {

// Deserialization functions

{{#FromJsonImpl}}
void from_json(nlohmann::json const& json, {{ComponentName}}& component) {
	auto c = json.find("{{ComponentName}}Component");
	if (c == json.end()) {
		throw std::runtime_error("No {{ComponentName}} component stored even though it was requested");
	} else {
		{{#DeserializeField}}
		if (auto x = (*c).find("{{FieldNameJson}}"); x != (*c).end()) {
			component.{{FieldName}} = (*x).get<{{{FieldType}}}>();
		}
		{{/DeserializeField}}
	}
}

{{/FromJsonImpl}}

{{#ToJsonImpl}}
void to_json(nlohmann::json& json, {{ComponentName}} const& component) {
	json["{{ComponentName}}Component"] = nlohmann::json::object();
	// clang-format off
	{{#SerializeField}}
	json["{{ComponentName}}Component"]["{{FieldNameJson}}"] = component.{{FieldName}};
	{{/SerializeField}}
	// clang-format on
}

{{/ToJsonImpl}}

} // namespace Saturn::Components